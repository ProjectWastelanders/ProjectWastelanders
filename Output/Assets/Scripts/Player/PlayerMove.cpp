#include "PlayerMove.h"
#include "../SwapCam.h"
#include "../UI Test folder/SwapWeapon.h"

HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();

    script->AddDragFloat("Velocity", &classInstance->vel);
    script->AddDragFloat("Upgrade Velocity", &classInstance->upgradedVel);
    script->AddDragFloat("SecToMaxVel", &classInstance->secToMaxVel);
    script->AddDragFloat("SecToZeroVel", &classInstance->secToZeroVel);
    //script->AddDragFloat("Current Velocity", &classInstance->currentVel);
    script->AddDragFloat("Y tp limit", &classInstance->yTpLimit);
    script->AddDragBoxRigidBody("Rigid Body", &classInstance->rigidBody);

    script->AddDragFloat("Dash Time", &classInstance->dashTime);
    script->AddDragFloat("Dash Distance", &classInstance->dashDistance);
    script->AddDragFloat("Upgrade Dash Distance", &classInstance->upgradedDashDistance);
    script->AddDragFloat("Dash Cooldown", &classInstance->maxDashCooldown);
    script->AddDragFloat("Upgrade Dash Cooldown", &classInstance->maxFastDashCooldown);

    script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->playerAnimator);
    script->AddDragBoxAnimationResource("Dash Animation", &classInstance->dashAnim);
    script->AddDragBoxAnimationResource("Idle Animation 1", &classInstance->idle1Anim);
    script->AddDragBoxAnimationResource("Idle Animation 2", &classInstance->idle2Anim);
    script->AddDragBoxAnimationResource("Idle Animation 3", &classInstance->idle3Anim);
    script->AddDragBoxAnimationResource("Run Forward Animation", &classInstance->runForwardAnim);
    script->AddDragBoxAnimationResource("Run Back Animation", &classInstance->runBackAnim);
    script->AddDragBoxAnimationResource("Run Left Animation", &classInstance->runLeftAnim);
    script->AddDragBoxAnimationResource("Run Right Animation", &classInstance->runRightAnim);
    script->AddDragBoxAnimationResource("Shoot Duals Animation", &classInstance->shootAnim[0]);
    script->AddDragBoxAnimationResource("Shoot SemiAuto Animation", &classInstance->shootAnim[1]);
    script->AddDragBoxAnimationResource("Shoot Automatic Animation", &classInstance->shootAnim[2]);
    script->AddDragBoxAnimationResource("Shoot Burst Animation", &classInstance->shootAnim[3]);
    script->AddDragBoxAnimationResource("Shoot Shotgun Animation", &classInstance->shootAnim[4]);
    script->AddDragBoxAnimationResource("Shoot Flamethrower Animation", &classInstance->shootAnim[5]);
    script->AddDragBoxAnimationResource("Shoot Ricochet Animation", &classInstance->shootAnim[6]);
    script->AddDragBoxAnimationResource("Swap Duals Animation", &classInstance->swapGunAnim[0]);
    script->AddDragBoxAnimationResource("Swap Gun Animation", &classInstance->swapGunAnim[1]);
    script->AddDragBoxAnimationResource("Hit Animation", &classInstance->hittedAnim);
    script->AddDragBoxAnimationResource("Open Chest Animation", &classInstance->openChestAnim);
    script->AddDragBoxAnimationResource("Dead Animation", &classInstance->deathAnim);
    script->AddDragBoxAnimationResource("Jumper Animation", &classInstance->jumperAnim);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragBoxParticleSystem("Walk Particles", &classInstance->walkParticles);
    script->AddDragBoxParticleSystem("Shoot Particles", &classInstance->shootParticles);
    return classInstance;
}

void PlayerMove::Start()
{
    transform = gameObject.GetTransform();
    initialPos = transform.GetGlobalPosition();
    departureTime = 0.0f;
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on PlayerMove Script.");

    if (playerStats && playerStats->movementTreeLvl > 3) dashesAvailable = 2;
    else dashesAvailable = 1;
    dashBuffer = false;

    impulseTime = 0.0f;
}

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();

    if (playerStats && !playerStats->PlayerAlive()) return;

    //Void tp
    if (transform.GetGlobalPosition().y < yTpLimit) transform.SetPosition(initialPos);

    if (moveSoundCooldown > 0.0f)
    {
        moveSoundCooldown -= dt;
        if (moveSoundCooldown <= 0.0f) moveSoundCooldown = 0.0f;
    }

    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    // impulse
    if (impulseTime > 0.0f)
    {
        rigidBody.SetVelocity(impulseDirection * impulseStrenght);
        impulseTime -= dt;

        if (impulseTime <= 0.0f)
        {
            impulseTime = 0.0f;
        }
        else
        {
            return; // can't do other actions while is been impulsed
        }
    }

    if (openingChest || (playerStats && playerStats->hittedTime > 0.0f)) return; // can't do other actions while is opening a chest or been hitted

    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) < 5000 || isSwapingGun)
    {
        isShooting = false;
        shootParticles.StopEmitting();
    }

    if (dashesAvailable > 0)
    {
        if (DashInput())
        {
            if (isDashing)
            {
                dashBuffer = true;
            }
            else
            {
                DashSetup();
            }
        }
        else if (dashBuffer && !isDashing)
        {
            DashSetup();
            dashBuffer = false;
        }
    }

    // refresh dash
    if (dashCooldown > 0.0f)
    {
        dashCooldown -= dt;
        if (dashCooldown <= 0.0f)
        {
            dashCooldown = 0.0f;
            if (playerStats && playerStats->movementTreeLvl > 3) dashesAvailable = 2;
            else dashesAvailable = 1;
        }
    }

    if (isDashing)
    {
        Dash();
        return; //NO MORE MOVEMENT
    }

    Aim();

    API_Vector2 input = GetMoveInput();
    //currentInput = input.Distance(API_Vector2::S_Zero());   //TEST

    float velocity;
    if (playerStats && playerStats->movementTreeLvl > 0) velocity = upgradedVel;
    else velocity = vel;

    if (playerStats && playerStats->speedPowerUp > 0.0f) velocity *= 1.5f;

    //SecToZero MUST be smaller than SecToMaxVel
    if (abs(input.x) < 0.01f && abs(input.y) < 0.01f) //NO INPUT
    {
        if (departureTime > secToZeroVel) departureTime = secToZeroVel;

        if (departureTime > 0.0f) {
            currentVel = Lerp(0.0f, velocity, departureTime / secToZeroVel);
            departureTime -= dt;
            input = lastMovInput;
        }
        else {
            currentVel = 0.0f;
        }
        if (playingWalkParticles)
        {
            walkParticles.StopEmitting();
            playingWalkParticles = false;
        }
    }
    else //MOVEMENT
    {
        if (departureTime < 0.0f) departureTime = 0.0f;

        if (departureTime < secToMaxVel) {
            currentVel = Lerp(0.0f, velocity, departureTime / secToMaxVel);
            departureTime += dt;
        }
        else {
            currentVel = velocity;
        }
        lastMovInput = input;
        if (moveSoundCooldown == 0.0f)
        {
            moveSoundCooldown = 0.5f;
            Audio::Event("starlord_walk");
        }
        if (!playingWalkParticles)
        {
            walkParticles.Play();
            playingWalkParticles = true;
        }
    }

    input *= currentVel;
    rigidBody.SetVelocity(API_Vector3(input.x, 0.0f, input.y));

    if (specialIdleActive && specialIdleTime > 0.0f)
    {
        specialIdleTime -= dt;

        if (specialIdleTime <= 0.0f)
        {
            specialIdleActive = false;
            specialIdleTime = 0.0f;
        }
    }

    if (currentVel <= 0.0f && currentAnim != PlayerAnims::IDLE && !isShooting && !isSwapingGun) //NO INPUT
    {
        float random = rand() % 100;
        if (random < 5.0f)
        {
            playerAnimator.ChangeAnimation(idle2Anim);
            specialIdleActive = true;
        }
        else if (random < 10.0f)
        {
            playerAnimator.ChangeAnimation(idle3Anim);
            specialIdleActive = true;
        }
        else
        {
            playerAnimator.ChangeAnimation(idle1Anim);
        }
        playerAnimator.Play();
        currentAnim = PlayerAnims::IDLE;
    }
    else if (currentVel <= 0.0f && specialIdleTime == 0.0f && !isShooting && !isSwapingGun) //NO INPUT
    {
        specialIdleTime = 1.0f;
        playerAnimator.ChangeAnimation(idle1Anim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::IDLE;
    }
}

float PlayerMove::Lerp(float a, float b, float time)
{
    return a + time * (b - a);
}

void PlayerMove::DashSetup()
{
    isDashing = true;

    // cooldown
    dashesAvailable--;
    if (playerStats && playerStats->movementTreeLvl > 1) dashCooldown = maxFastDashCooldown + 0.0001f;
    else dashCooldown = maxDashCooldown + 0.0001f;
    
    dashDepartTime = 0.0f;
    float norm = sqrt(pow(lastMovInput.x, 2) + pow(lastMovInput.y, 2));
    API_Vector3 movDir;
    movDir.x = lastMovInput.x / norm;
    movDir.y = 0.0f;
    movDir.z = lastMovInput.y / norm;

    float dist = dashDistance;
    if (playerStats && playerStats->movementTreeLvl > 2) dist = upgradedDashDistance;

    //Set dash vel
    rigidBody.SetVelocity((movDir * dist) / dashTime);
    transform.SetRotation(0.0f, atan2(movDir.x, movDir.z) * RADTODEG, 0.0f);

    if (currentAnim != PlayerAnims::DASH)
    {
        playerAnimator.ChangeAnimation(dashAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::DASH;
    }
    playerStats->inmunityTime = dashTime;
    Audio::Event("starlord_dash");
}

void PlayerMove::Dash()
{
    dashDepartTime += dt;

    if (dashDepartTime >= dashTime)
    {
        isDashing = false;
    }
}

bool PlayerMove::DashInput()
{
    if (usingGamepad)
        return Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERLEFT) > 20000;

    return Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN;
}

void PlayerMove::LookAt(API_Vector3 target)
{
    API_Vector2 movDir = {target.x, target.z};

    float norm = sqrt(target.x * target.x + target.y * target.y);
    movDir /= norm;

    transform.SetRotation(0.0f, atan2(movDir.x, movDir.y) * RADTODEG, 0.0f);

    //Maintain Aiming
    lastAimInput = movDir;
}

void PlayerMove::Aim()
{
    API_Vector2 normalizedInput;

    if (usingGamepad)
    {
        API_Vector2 input;
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX);
        input.y = -Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY);

        if (abs(input.x) < 10000 && abs(input.y) < 10000) return;

        float norm = sqrt(pow(input.x, 2) + pow(input.y, 2));
        normalizedInput.x = input.x / norm;
        normalizedInput.y = input.y / norm;
    }
    else
    {
        normalizedInput.x = Input::GetKey(KeyCode::KEY_RIGHT) == KeyState::KEY_REPEAT;
        normalizedInput.x -= Input::GetKey(KeyCode::KEY_LEFT) == KeyState::KEY_REPEAT;
        normalizedInput.y = Input::GetKey(KeyCode::KEY_UP) == KeyState::KEY_REPEAT;
        normalizedInput.y -= Input::GetKey(KeyCode::KEY_DOWN) == KeyState::KEY_REPEAT;

        if (normalizedInput.x != 0.0f && normalizedInput.y != 0.0f) {
            // sqrt(2)/2 = 0.71
            normalizedInput.x *= 0.71f;
            normalizedInput.y *= 0.71f;
        }
    }

    //Get last input to keep Aim angle
    if (normalizedInput.x == 0.0f && normalizedInput.y == 0.0f)
        normalizedInput = lastAimInput;
    else
        lastAimInput = normalizedInput;

    aimAngle = atan2(normalizedInput.y, normalizedInput.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, aimAngle, 0);
}

API_Vector2 PlayerMove::GetMoveInput()
{
    API_Vector2 input;

    //Gamepad
    if (usingGamepad) {
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX);
        input.y = Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY);

        if (abs(input.x) < 10000 && abs(input.y) < 10000) return API_Vector2(0.0f, 0.0f);

        if (input.x > 32000.0f) input.x = 32000.0f;
        else if (input.x < -32000.0f) input.x = -32000.0f;
        if (input.y > 32000.0f) input.y = 32000.0f;
        else if (input.y < -32000.0f) input.y = -32000.0f;

        // run animation
        if (abs(input.y) > abs(input.x))
        {
            if (input.y < 0.0f) // walk forward
            {
                if (aimAngle <= 45 && aimAngle > -45) // move forward
                {
                    if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runForwardAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_FORWARD;
                    }
                }
                else if (aimAngle <= -45 && aimAngle > -135) // move left
                {
                    if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runLeftAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_LEFT;
                    }
                }
                else if (aimAngle <= -135 && aimAngle > -225) // move back
                {
                    if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runBackAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_BACK;
                    }
                }
                else // move right
                {
                    if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runRightAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_RIGHT;
                    }
                }
            }
            else if (input.y > 0.0f) // walk back
            {
                if (aimAngle <= 45 && aimAngle > -45) // move back
                {
                    if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runBackAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_BACK;
                    }
                }
                else if (aimAngle <= -45 && aimAngle > -135) // move right
                {
                    if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runRightAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_RIGHT;
                    }
                }
                else if (aimAngle <= -135 && aimAngle > -225) // move forward
                {
                    if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runForwardAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_FORWARD;
                    }
                }
                else // move left
                {
                    if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runLeftAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_LEFT;
                    }
                }
            }
        }
        else
        {
            if (input.x > 0.0f) // walk right
            {
                if (aimAngle <= 45 && aimAngle > -45) // move right
                {
                    if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runRightAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_RIGHT;
                    }
                }
                else if (aimAngle <= -45 && aimAngle > -135) // move forward
                {
                    if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runForwardAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_FORWARD;
                    }
                }
                else if (aimAngle <= -135 && aimAngle > -225) // move left
                {
                    if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runLeftAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_LEFT;
                    }
                }
                else // move back
                {
                    if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runBackAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_BACK;
                    }
                }
            }
            else if (input.x < 0.0f) // walk left
            {
                if (aimAngle <= 45 && aimAngle > -45) // move left
                {
                    if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runLeftAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_LEFT;
                    }
                }
                else if (aimAngle <= -45 && aimAngle > -135) // move back
                {
                    if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runBackAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_BACK;
                    }
                }
                else if (aimAngle <= -135 && aimAngle > -225) // move right
                {
                    if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runRightAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_RIGHT;
                    }
                }
                else // move forward
                {
                    if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                    {
                        playerAnimator.ChangeAnimation(runForwardAnim);
                        playerAnimator.Play();
                        currentAnim = PlayerAnims::RUN_FORWARD;
                    }
                }
            }
        }

        return -input / 32000.0f;
    }

    //Keyboard
    input.x = Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT;
    input.x -= Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT;
    input.y = Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT;
    input.y -= Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT;

    // run animation
    if (abs(input.y) > abs(input.x))
    {
        if (input.y == -1.0f) // walk forward
        {
            if (aimAngle <= 45 && aimAngle > -45) // move forward
            {
                if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runForwardAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_FORWARD;
                }
            }
            else if (aimAngle <= -45 && aimAngle > -135) // move left
            {
                if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runLeftAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_LEFT;
                }
            }
            else if (aimAngle <= -135 && aimAngle > -225) // move back
            {
                if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runBackAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_BACK;
                }
            }
            else // move right
            {
                if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runRightAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_RIGHT;
                }
            }
        }
        else if (input.y == 1.0f) // walk back
        {
            if (aimAngle <= 45 && aimAngle > -45) // move back
            {
                if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runBackAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_BACK;
                }
            }
            else if (aimAngle <= -45 && aimAngle > -135) // move right
            {
                if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runRightAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_RIGHT;
                }
            }
            else if (aimAngle <= -135 && aimAngle > -225) // move forward
            {
                if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runForwardAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_FORWARD;
                }
            }
            else // move left
            {
                if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runLeftAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_LEFT;
                }
            }
        }
    }
    else
    {
        if (input.x == -1.0f) // walk right
        {
            if (aimAngle <= 45 && aimAngle > -45) // move right
            {
                if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runRightAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_RIGHT;
                }
            }
            else if (aimAngle <= -45 && aimAngle > -135) // move forward
            {
                if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runForwardAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_FORWARD;
                }
            }
            else if (aimAngle <= -135 && aimAngle > -225) // move left
            {
                if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runLeftAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_LEFT;
                }
            }
            else // move back
            {
                if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runBackAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_BACK;
                }
            }
        }
        else if (input.x == 1.0f) // walk left
        {
            if (aimAngle <= 45 && aimAngle > -45) // move left
            {
                if (currentAnim != PlayerAnims::RUN_LEFT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runLeftAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_LEFT;
                }
            }
            else if (aimAngle <= -45 && aimAngle > -135) // move back
            {
                if (currentAnim != PlayerAnims::RUN_BACK && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runBackAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_BACK;
                }
            }
            else if (aimAngle <= -135 && aimAngle > -225) // move right
            {
                if (currentAnim != PlayerAnims::RUN_RIGHT && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runRightAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_RIGHT;
                }
            }
            else // move forward
            {
                if (currentAnim != PlayerAnims::RUN_FORWARD && !isShooting && !isSwapingGun)
                {
                    playerAnimator.ChangeAnimation(runForwardAnim);
                    playerAnimator.Play();
                    currentAnim = PlayerAnims::RUN_FORWARD;
                }
            }
        }
    }

    //Diagonal movement
    if (input.x != 0.0f && input.y != 0.0f) {
        // sqrt(2)/2 = 0.71
        input.x *= 0.71f;
        input.y *= 0.71f;
    }

    return input;
}

void PlayerMove::RecieveImpulse(API_Vector3 direction, float impulseDuration, float impulseForce)
{
    impulseDirection = direction;
    impulseTime = impulseDuration;
    impulseStrenght = impulseForce;
}

void PlayerMove::PlayShootAnim(int gunIndex)
{
    isShooting = true;
    
    if (currentAnim != PlayerAnims::SHOOT)
    {
        shootParticles.Play();
        playerAnimator.ChangeAnimation(shootAnim[gunIndex]);
        playerAnimator.Play();
        currentAnim = PlayerAnims::SHOOT;
    }
}

void PlayerMove::PlaySwapGunAnim(int animationIndex)
{
    isSwapingGun = true;

    if (currentAnim != PlayerAnims::SWAP_GUN)
    {
        playerAnimator.ChangeAnimation(swapGunAnim[animationIndex]);
        playerAnimator.Play();
        currentAnim = PlayerAnims::SWAP_GUN;
    }
}

void PlayerMove::StopSwapGunAnim()
{
    isSwapingGun = false;
}

void PlayerMove::PlayHittedAnim()
{
    if (currentAnim != PlayerAnims::HITTED)
    {
        playerAnimator.ChangeAnimation(hittedAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::HITTED;
    }
}

void PlayerMove::PlayOpenChestAnim()
{
    if (currentAnim != PlayerAnims::OPEN_CHEST)
    {
        playerAnimator.ChangeAnimation(openChestAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::OPEN_CHEST;
        openingChest = true;
    }
}

void PlayerMove::StopOpenChestAnim()
{
    openingChest = false;
}

void PlayerMove::PlayDeathAnim()
{
    if (currentAnim != PlayerAnims::DEATH)
    {
        playerAnimator.ChangeAnimation(deathAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::DEATH;
    }
}

void PlayerMove::PlayJumperAnim()
{
    if (currentAnim != PlayerAnims::JUMPER)
    {
        playerAnimator.ChangeAnimation(jumperAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::JUMPER;
    }
}
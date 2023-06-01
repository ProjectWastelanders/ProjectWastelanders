
import csv
import json

class MyCustomClass:
    def __init__(self, name, role, photo, email, github, linkedin, brief, contributions):
        self.name = name
        self.role = role
        self.photo = photo
        self.email = email
        self.github = github
        self.linkedin = linkedin
        self.brief = brief
        self.contributions = contributions
#
data = []
with open('test2.csv', 'r') as file:
    reader = csv.DictReader(file)
    print(reader.fieldnames)
    for row in reader:
        entry=MyCustomClass(
            name=row['Nombre'],
            role=row['Rol (Design, Art, Code, Production)'],
            photo=row['Foto de perfil (nombre del archivo) Ejemplo: (Nombre_Apellido.png)'],
            email=row['Xarxes socials: e-mail de contacte'],
            github=row['Xarxes socials: github'],
            linkedin=row['Xarxes socials: linkedin'],
            brief=row['Comentari general (1 parÃ\xa0graf)'],
            contributions=row['Tasques principals (4 punts)'],)
        data.append(entry.__dict__)
with open('team_data.json', 'w', encoding='utf-16') as f:
    json.dump(data, f, ensure_ascii=False, indent=4)
#
html = []
def fill_html(member):
    entry = '''
            <div class="col span_1_of_2 mix entry4role box">
                <img src="resources/img/entry4photo" alt="Name 1" class="team-member">
                <li><a class="myBtn_multi">entry4name</a></li>
                <span class="role">entry4role</span>
            </div>
            <!-- The Modal 1-->
            <div class="modal modal_multi">

                <!-- Modal content -->
                <div class="modal-content">
                    <span class="close close_multi"></span>
                    <div class="row">
                        <div class="col span_1_of_2 box">
                            <h4>General Info</h4>
                            <p>entry4brief</p>

                            <h4> Tasks Overview</h4>
                            <ul>
                                <li>
                                    <p>entry4tasks</p>
                                </li>
                            </ul>

                            <div class="social-link">
                                <ul>
                                    <li><a href="entry4github"><i class="fab fa-github"></i></a></li>
                                    <li><a href="entry4linkedin"><i class="fab fa-linkedin-in"></i></a></li>
                                    <li><a href="entry4email"><i class="fas fa-envelope"></i></a></li>
                                </ul>
                            </div>
                        </div>
                        <div class="col span_1_of_2 box">
                            <img src="resources/img/entry4photo" alt="Name 4" class="team-member">
                            <h3>entry4name</h3>
                        </div>
                    </div>
                </div>

            </div>

    '''
    entry = entry.replace('entry4brief', member['brief'])
    entry = entry.replace('entry4tasks', member['contributions'])
    entry = entry.replace('entry4email', member['email'])
    entry = entry.replace('entry4github', member['github'])
    entry = entry.replace('entry4linkedin', member['linkedin'])
    entry = entry.replace('entry4photo', member['photo'])
    entry = entry.replace('entry4name', member['name'])
    entry = entry.replace('entry4role', member['role'].lower().replace(",", "").replace(" ", " "))
    html.append(entry)
#
for member in data:
    fill_html(member)
#
with open('fillhtml.txt', 'w', encoding='utf-16') as file:
    content = '\n'.join(html)
    file.write(content)
#
print('done!')

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
with open('test2.csv', 'r', encoding='utf-8') as file:
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
            brief=row['Comentari general (1 paràgraf)'],
            contributions=row['Tasques principals (4 punts)'],)
        data.append(entry.__dict__)
with open('team_data.json', 'w', encoding='utf-8') as f:
    json.dump(data, f, ensure_ascii=False, indent=4)
#
html = []
def fill_html(member):
    entry4name = member['name'].replace('\n','')
    entry4role = member['role'].replace('\n','').lower().replace(' ','').replace(',',' ')
    entry4blueRole = 'Lead' if 'production' in entry4role else ''
    entry4photo = 'resources/img/' + member['photo'].replace('\n','')
    entry4email = member['email'].replace('\n','')
    entry4github = member['github'].replace('\n','')
    entry4linkedin = member['linkedin'].replace('\n','')
    entry4brief = '\n'.join(['<p>\n'+p+'\n</p>' for p in member['brief'].split('\n') if p])
    entry4contributions = '\n'.join(['<li><p>'+p+'</p></li>' for p in member['contributions'].split('\n') if p])
    #
    entry = '''
    <div class="col span_1_of_2 mix entry4role box">
        <img src="entry4photo" alt="Name 1" class="team-member">
        <li><a class="myBtn_multi">entry4name</a></li>
        <span class="role">entry4blueRole</span>
    </div>
    <div class="modal modal_multi">
        <div class="modal-content">
            <span class="close close_multi"></span>
            <div class="row">
                <div class="col span_1_of_2 box">
                    <h4>General Info</h4>
                    entry4brief
                    <h4> Tasks Overview</h4>
                    <ul>
                        entry4contributions
                    </ul>
                </div>
                <div class="col span_1_of_2 box">
                    <img src="entry4photo" alt="Name 4" class="team-member">
                    <h3>entry4name</h3>
                    <span class="role">entry4role</span>
                    <div class="social-link">
                        <ul>
                            <li><a href="entry4github"><i class="fab fa-github"></i></a></li>
                            <li><a href="entry4linkedin"><i
                                        class="fab fa-linkedin-in"></i></a></li>
                        </ul>
                        <span class="mail">entry4email</span>
                    </div>
                </div>
            </div>
        </div>
    </div>
    '''
    #
    entry = entry.replace('entry4name', entry4name)
    entry = entry.replace('entry4role', entry4role)
    entry = entry.replace('entry4blueRole', entry4blueRole)
    entry = entry.replace('entry4photo', entry4photo)
    entry = entry.replace('entry4email', entry4email)
    entry = entry.replace('entry4github', entry4github)
    entry = entry.replace('entry4linkedin', entry4linkedin)
    entry = entry.replace('entry4brief', entry4brief)
    entry = entry.replace('entry4contributions', entry4contributions)
    #
    html.append(entry)
#
for member in data:
    fill_html(member)
#
with open('fill_html.html', 'w', encoding='utf-8') as file:
    content = '\n'.join(html)
    file.write(content)
#
print('done! --remember to format pretty')
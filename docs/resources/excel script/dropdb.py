
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
with open('resources/excel script/final.csv', 'r', encoding='utf-8') as file:
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
def clean_str(str):
    ''.join(str.replace('\t', ' ').split(' '))
#
html = []
def fill_html(member):
    entry4name = member['name'].replace('\n','')
    clean_str(entry4name)
    name_parts = member['name'].split(', ')
    entry4secondName = name_parts[1] + ' ' + name_parts[0]
    clean_str(entry4secondName)
    entry4role = member['role'].replace('\n','').lower().replace(' ','').replace(',',' ')
    clean_str(entry4role)
    entry4commaRole = entry4role.title().replace(' ', ', ')
    clean_str(entry4commaRole)
    entry4blueRole = 'Lead' if 'production' in entry4role else ''
    clean_str(entry4blueRole)
    entry4photo = 'resources/img/Profile_Pictures/' + member['photo'].replace('\n','')
    clean_str(entry4photo)
    entry4email = member['email'].replace('\n','')
    clean_str(entry4email)
    entry4github = member['github'].replace('\n','')
    clean_str(entry4github)
    entry4linkedin = member['linkedin'].replace('\n','')
    clean_str(entry4linkedin)
    entry4brief = '\n'.join(['<p>\n'+p+'\n</p>' for p in member['brief'].split('\n') if p])
    clean_str(entry4brief)
    entry4contributions = '\n'.join(['<li><p>'+p+'</p></li>' for p in member['contributions'].split('\n') if p])
    clean_str(entry4contributions)
    #
    
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
                <div class="col span_1_of_2 box" style="margin-bottom: 3rem;">
                    <h4>General Info</h4>
                    entry4brief
                    <h4 style="margin-top: 5rem;"> Tasks Overview</h4>
                    <ul>
                        entry4contributions
                    </ul>
                </div>
                <div class="col span_1_of_2 box">
                    <img src="entry4photo" alt="Name 4" class="team-member">
                    <h3>entry4secondName</h3>
                    <span class="role">entry4commaRole</span>
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
    entry = entry.replace('entry4name', entry4secondName)#last fix
    entry = entry.replace('entry4secondName', entry4secondName)
    entry = entry.replace('entry4role', entry4role)
    entry = entry.replace('entry4commaRole', entry4commaRole)
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
with open('resources/excel script/fill.html', 'w', encoding='utf-8') as file:
    content = '\n'.join(html)
    file.write(content)
#
print('done! --remember to format pretty')
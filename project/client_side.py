# import requests
# from threading import Thread

# SERVER_URL = "http://localhost:8080/index.cgi"

# NUM_CLIENTS = 3

# def launch_request():
#     response = requests.get(SERVER_URL)
#     print(response.status_code)

# threads = [Thread(target=launch_request) for _ in range(NUM_CLIENTS)]

# for t in threads:
#     t.run()

# print("Test finished")


import requests
from threading import Thread


URLS = [
    "http://localhost:8080",
    "http://localhost:8081",
    "http://localhost:8082",
]

CLIENTS_PER_URL = 3

def launch_request(url):
    response = requests.get(url)
    print(response.status_code)

threads = [Thread(target=launch_request, args=(url,)) 
            for url in URLS for _ in range(CLIENTS_PER_URL)]

for t in threads:
    t.start()

for t in threads:
    t.join()

print("Test finished")
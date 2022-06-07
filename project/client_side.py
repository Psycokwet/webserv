import requests
from threading import Thread

SERVER_URL = "http://localhost:8080/index.cgi"

NUM_CLIENTS = 3

def launch_request():
    response = requests.get(SERVER_URL)
    print(response.status_code)

threads = [Thread(target=launch_request) for _ in range(NUM_CLIENTS)]

for t in threads:
    t.run()

print("Test finished")
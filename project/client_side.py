import requests
from threading import Thread

SERVER_URL = "http://localhost:8080"

NUM_CLIENTS = 30

def launch_request():
    response = requests.get(SERVER_URL)
    print(response.status_code)

threads = [Thread(target=launch_request) for _ in range(NUM_CLIENTS)]

for t in threads:
    t.run()

for t in threads:
    t.join()

print("Test finished")
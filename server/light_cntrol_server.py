from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs, urlparse
from datetime import time, datetime


address = ('192.168.1.13', 8080)
def is_open():
    night = time(22,00,0)
    current_time = datetime.now().time()
    return current_time < night 

class MyHTTPRequestHandler(BaseHTTPRequestHandler):


    def do_GET(self):
        parsed_path = urlparse(self.path)
     
        sensor_value = int(parse_qs(parsed_path.query)['data'][0])
        light= 'ON' if ((sensor_value <3000) and is_open()) else 'OFF'

        self.send_response(200)
        self.send_header('Content-Type', 'text/plain; charset=utf-8')
        self.end_headers()
        self.wfile.write(light.encode())

with HTTPServer(address, MyHTTPRequestHandler) as server:
    server.serve_forever()

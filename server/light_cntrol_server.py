from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs, urlparse

address = ('192.168.1.13', 8080)

class MyHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urlparse(self.path)
     
        sensor_value = int(parse_qs(parsed_path.query)['data'][0])
        light= 'ON' if sensor_value <3000 else 'OFF'

        self.send_response(200)
        self.send_header('Content-Type', 'text/plain; charset=utf-8')
        self.end_headers()
        self.wfile.write(light.encode())

with HTTPServer(address, MyHTTPRequestHandler) as server:
    server.serve_forever()

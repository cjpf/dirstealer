import http.client

print("** This program sends a GET request on a user-specified resource. It checks the status code return in the response using the function named status()  **\n")

host = input("Insert the host/IP: ")
port = input("Insert the port(default:80): ")
resource = input("Insert resource to GET: ")

if(port == ""):
    port = 80

try:
    connection = http.client.HTTPConnection(host, port)
    connection.request('GET', resource)
    status = connection.getresponse().status
    print("The status returned for that request is: ",status)
    connection.close()
except ConnectionRefusedError:
    print("Connection failed")
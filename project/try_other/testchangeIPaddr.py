import ipaddress
  
# converting IPv4 address to ints
print("Add an IP address")
for i in range(10):
    str = input()
    addr = ipaddress.ip_address(str)
    print("IP address in int type is: {}".format(int(addr)))
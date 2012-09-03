INTIF="br0"
EXTIF="eth6"
echo 1 > /proc/sys/net/ipv4/ip_forward

#crete bridge
brctl addbr br0
tunctl -t tap0
ifconfig tap0 up
brctl addif br0 tap0

# clear existing iptable rules, set a default policy
iptables -P INPUT ACCEPT
iptables -F INPUT 
iptables -P OUTPUT ACCEPT
iptables -F OUTPUT 
iptables -P FORWARD DROP
iptables -F FORWARD 
iptables -t nat -F

# set forwarding and nat rules
iptables -A FORWARD -i $EXTIF -o $INTIF -j ACCEPT
iptables -A FORWARD -i $INTIF -o $EXTIF -j ACCEPT
iptables -t nat -A POSTROUTING -o $EXTIF -j MASQUERADE

#Setting will be do with

# clear existing iptable rules, set a default policy
#iptables -P INPUT ACCEPT
#iptables -F INPUT 
#iptables -P OUTPUT ACCEPT
#iptables -F OUTPUT 
#iptables -P FORWARD DROP
#iptables -F FORWARD 
#iptables -t nat -F
#
## disable forwarding
#echo 0 > /proc/sys/net/ipv4/ip_forward
#echo 1 > /proc/sys/net/ipv4/ip_dynaddr
#

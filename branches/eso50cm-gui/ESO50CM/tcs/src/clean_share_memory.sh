# run this script as root
#!/bin/bash

ipcrm -S 0xfafafa00
ipcrm -S 0xfafafa01
ipcrm -S 0xfafafa02

ipcrm -M 0xfafafa00
ipcrm -M 0xfafafa01



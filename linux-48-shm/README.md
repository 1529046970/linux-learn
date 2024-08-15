$ make
$ wc -c /etc/services

$ ./writer < /etc/services &

$ ./reader > out.txt
$ diff /etc/services out.txt
$

#diff output nothing....







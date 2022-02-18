test () {
	./microshell $@ > out 2> err
	./glagan $@ > out2 2> err2
	diff out out2
	diff err err2
	rm out out2 err err2
}

test /bin/ls
test /bin/cat microshell.c
test /bin/ls microshell.c
test /bin/ls salut
test ";"
test ";" ";"
test ";" ";" /bin/echo OK
test ";" ";" /bin/echo OK ";"
test ";" ";" /bin/echo OK ";" ";"
test ";" ";" /bin/echo OK ";" ";" ";" /bin/echo OK
test /bin/ls "|" /usr/bin/grep microshell
test /bin/ls "|" /usr/bin/grep microshell "|" /usr/bin/grep micro
test /bin/ls "|" /usr/bin/grep microshell "|" /usr/bin/grep micro "|" /usr/bin/grep shell "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro
test /bin/ls "|" /usr/bin/grep microshell "|" /usr/bin/grep micro "|" /usr/bin/grep shell "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep micro "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell "|" /usr/bin/grep shell
test /bin/ls ewqew "|" /usr/bin/grep micro "|" /bin/cat -n ";" /bin/echo dernier ";" /bin/echo
test /bin/ls "|" /usr/bin/grep micro "|" /bin/cat -n ";" /bin/echo dernier ";" /bin/echo ftest ";"
test /bin/echo ftest ";" /bin/echo ftewerwerwerst ";" /bin/echo werwerwer ";" /bin/echo qweqweqweqew ";" /bin/echo qwewqeqrtregrfyukui ";"
test /bin/ls ftest ";" /bin/ls ";" /bin/ls werwer ";" /bin/ls microshell.c ";" /bin/ls subject.fr.txt ";"
test /bin/ls "|" /usr/bin/grep micro ";" /bin/ls "|" /usr/bin/grep micro ";" /bin/ls "|" /usr/bin/grep micro ";" /bin/ls "|" /usr/bin/grep micro ";"
test /bin/cat subject.fr.txt "|" /usr/bin/grep a "|" /usr/bin/grep b ";" /bin/cat subject.fr.txt ";"
test /bin/cat subject.fr.txt "|" /usr/bin/grep a "|" /usr/bin/grep w ";" /bin/cat subject.fr.txt ";"
test /bin/cat subject.fr.txt "|" /usr/bin/grep a "|" /usr/bin/grep w ";" /bin/cat subject.fr.txt
test /bin/cat subject.fr.txt ";" /bin/cat subject.fr.txt "|" /usr/bin/grep a "|" /usr/bin/grep b "|" /usr/bin/grep z ";" /bin/cat subject.fr.txt
test ";" /bin/cat subject.fr.txt ";" /bin/cat subject.fr.txt "|" /usr/bin/grep a "|" /usr/bin/grep b "|" /usr/bin/grep z ";" /bin/cat subject.fr.txt
test blah "|" /bin/echo OK
test blah "|" /bin/echo OK ";"
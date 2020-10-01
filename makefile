
output: main.o prompt.o syntax.o getcommands.o interpretcommands.o helperfunctions.o cd.o pwd.o echo.o ls.o pinfo.o nightswatch.o history.o setenv.o externalapi.o childhandler.o jobhandler.o redirection.o rawterminal.o
	gcc main.o prompt.o syntax.o getcommands.o interpretcommands.o helperfunctions.o cd.o pwd.o echo.o ls.o pinfo.o nightswatch.o history.o setenv.o externalapi.o childhandler.o jobhandler.o redirection.o rawterminal.o -o shell

main.o: main.c prompt.o pwd.o headers.h globalvars.h
	gcc -c main.c

prompt.o: prompt.c prompt.h getcommands.o externalapi.o childhandler.o history.o headers.h globalvars.h
	gcc -c prompt.c

syntax.o: syntax.c syntax.h helperfunctions.h headers.h
	gcc -c syntax.c

getcommands.o: getcommands.c getcommands.h helperfunctions.h interpretcommands.o history.o childhandler.o syntax.o headers.h globalvars.h
	gcc -c getcommands.c

interpretcommands.o: interpretcommands.c interpretcommands.h helperfunctions.o cd.o pwd.o echo.o ls.o pinfo.o nightswatch.o history.o setenv.o externalapi.o redirection.o childhandler.o jobhandler.o headers.h globalvars.h
	gcc -c interpretcommands.c

helperfunctions.o: helperfunctions.c helperfunctions.h headers.h
	gcc -c helperfunctions.c

cd.o: cd.c cd.h pwd.o helperfunctions.o headers.h globalvars.h
	gcc -c cd.c

pwd.o: pwd.c pwd.h headers.h
	gcc -c pwd.c

echo.o: echo.c echo.h headers.h
	gcc -c echo.c

ls.o: ls.c ls.h helperfunctions.o pwd.o headers.h globalvars.h
	gcc -c ls.c

pinfo.o: pinfo.c pinfo.h headers.h
	gcc -c pinfo.c

nightswatch.o: nightswatch.c nightswatch.h helperfunctions.o rawterminal.o headers.h
	gcc -c nightswatch.c

history.o: history.c history.h helperfunctions.o headers.h globalvars.h
	gcc -c history.c

setenv.o: setenv.c setenv.h headers.h
	gcc -c setenv.c

externalapi.o: externalapi.c externalapi.h helperfunctions.o headers.h globalvars.h
	gcc -c externalapi.c

childhandler.o: childhandler.c childhandler.h headers.h globalvars.h
	gcc -c childhandler.c

jobhandler.o: jobhandler.c jobhandler.h helperfunctions.o childhandler.o headers.h globalvars.h
	gcc -c jobhandler.c

redirection.o: redirection.c redirection.h helperfunctions.h headers.h
	gcc -c redirection.c

rawterminal.o: rawterminal.c rawterminal.h headers.h globalvars.h
	gcc -c rawterminal.c

clean:
	rm *.o shell
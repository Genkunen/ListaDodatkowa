BIN = bin

LISTA   = 1) lista dwukierunkowa
TABLICA = 2) tablica funkcji
BAZA    = 3) baza danych adresowych
HANOI   = 4) gra Hanoi
FRAKTAL = 5) rysowanie fraktali ASCII
ZYCIE   = 6) gra w zycie

default:
	-@mkdir $(BIN)

	@make compile -C "$(LISTA)"
	-@move /Y ".\$(LISTA)\*.exe" .\$(BIN)\
	
	@make compile -C "$(TABLICA)"
	-@move /Y ".\$(TABLICA)\*.exe" .\$(BIN)\ 
	
	@make compile -C "$(BAZA)"
	-@move /Y ".\$(BAZA)\*.exe" .\$(BIN)\
	
	@make compile -C "$(HANOI)"
	-@move /Y ".\$(HANOI)\*.exe" .\$(BIN)\
	
	@make compile -C "$(FRAKTAL)"
	-@move /Y ".\$(FRAKTAL)\*.exe" .\$(BIN)\
	
	@make compile -C "$(ZYCIE)"
	-@move /Y ".\$(ZYCIE)\*.exe" .\$(BIN)\

clean:
	@rmdir /s /q bin
DROP TABLE IF EXISTS Film CASCADE;
DROP TABLE IF EXISTS Kasjer CASCADE;
DROP TABLE IF EXISTS Klient CASCADE;
DROP TABLE IF EXISTS Rezerwacja CASCADE;
DROP TABLE IF EXISTS Sala CASCADE;
DROP TABLE IF EXISTS Seans CASCADE; 

CREATE TABLE Film (
    id SERIAL  PRIMARY KEY,
    tytuł varchar(100) NOT NULL,
    rok int NOT NULL,
    reżyser varchar(100) NOT NULL,
    czas_trwania int NOT NULL,
    gatunek varchar(100) NOT NULL,
    kraj varchar(20) NOT NULL,
    opis text NOT NULL,
    plakat varchar(300)
);

CREATE TABLE Kasjer (
    imię varchar(20) NOT NULL,
    nazwisko varchar(20) NOT NULL,    
    login varchar(20) PRIMARY KEY,
    hasło varchar(20) NOT NULL
);

CREATE TABLE Klient (
	id SERIAL PRIMARY KEY,
    imię varchar(30) NOT NULL,
    nazwisko varchar(30) NOT NULL,
    email varchar(30) NOT NULL
);

CREATE TABLE Sala (
    numer int PRIMARY KEY,
    liczba_miejsc int NOT NULL
);

CREATE TABLE Seans (
    id SERIAL PRIMARY KEY,
    film_id int NOT NULL REFERENCES Film,
    sala_nr int NOT NULL REFERENCES Sala,
    dzień_tygodnia smallint NOT NULL,
    CHECK (dzień_tygodnia >= 1 AND dzień_tygodnia <= 7),
    godzina_rozpoczęcia time NOT NULL,
    liczba_miejsc smallint NOT NULL
);

CREATE TABLE Rezerwacja (
    id SERIAL PRIMARY KEY,
    seans_id integer NOT NULL REFERENCES Seans(id),
    klient_id integer NOT NULL REFERENCES Klient(id),
    kasjer VARCHAR(20) REFERENCES Kasjer(login),
    liczba_miejsc integer NOT NULL
);
--kasjer_id == null jesli rezerwacja niepotwierdzona, 


CREATE OR REPLACE FUNCTION dzień_tygodnia_nazwa (integer) RETURNS varchar AS $$
DECLARE
	num ALIAS FOR $1;
	nazwa varchar;
BEGIN
	if num = 1 then
		nazwa := 'poniedziałek';
	elseif num = 2 then
		nazwa := 'wtorek';
	elseif num = 3 then			
		nazwa := 'środa';
	elseif num = 4 then
		nazwa := 'czwartek';
	elseif num = 5 then
		nazwa := 'piątek';
	elseif num = 6 then
		nazwa := 'sobota';
	else 
		nazwa := 'niedziela';
	end if;	
	RETURN nazwa;	
END;
$$ language plpgsql;		


CREATE OR REPLACE FUNCTION sprawdzKolizje () RETURNS TRIGGER AS $$
DECLARE
    koniec TIME;
    czas_pom INT;
BEGIN
    SELECT czas_trwania FROM Film JOIN Seans ON Seans.film_id = Film.id INTO czas_pom;
    koniec = NEW.godzina_rozpoczęcia + czas_pom * INTERVAL '1 minute';
    if not exists (
		    SELECT * FROM Seans JOIN Film ON Seans.film_id = Film.id
		    WHERE dzień_tygodnia = NEW.dzień_tygodnia AND sala_nr = NEW.sala_nr AND Seans.id != NEW.id
		    AND ((godzina_rozpoczęcia >= NEW.godzina_rozpoczęcia AND godzina_rozpoczęcia <= koniec) OR 
		         (godzina_rozpoczęcia + czas_trwania * INTERVAL '1 minute' >= NEW.godzina_rozpoczęcia 
		          AND godzina_rozpoczęcia + czas_trwania * INTERVAL '1 minute' <= koniec))
		    ) A
     	then RETURN NEW;
    else
    	raise exception 'seans koliduje z innym';
    end if;
END;
$$ language plpgsql;

CREATE TRIGGER sprawdzKolizjeT BEFORE INSERT OR UPDATE ON Seans
FOR EACH ROW 
EXECUTE PROCEDURE sprawdzKolizje();

-- po wstawieniu nowego seansu liczba wolnych miejsc musi byc taka sama jak w sali w ktorej sie odbywa
CREATE OR REPLACE FUNCTION liczba_miejsc() RETURNS TRIGGER AS $$
DECLARE
	wolne_miejsca integer;
BEGIN
	SELECT Sala.liczba_miejsc FROM Sala WHERE numer = NEW.sala_nr INTO wolne_miejsca;
	NEW.liczba_miejsc := wolne_miejsca;
	RETURN NEW;
END;
$$ language plpgsql;

CREATE TRIGGER liczba_miejscT BEFORE INSERT ON Seans
FOR EACH ROW 
EXECUTE PROCEDURE liczba_miejsc();


CREATE OR REPLACE FUNCTION zmniejsz_wolne_miejsca() RETURNS TRIGGER AS $$
DECLARE
	wolne_miejsca_stare integer;
	wolne_miejsca_nowe integer;
BEGIN
 	SELECT liczba_miejsc FROM Seans WHERE id = NEW.seans_id INTO wolne_miejsca_stare;
 	wolne_miejsca_nowe := wolne_miejsca_stare - NEW.liczba_miejsc;
 	if wolne_miejsca_nowe < 0 then
 		raise exception 'brak miejsc';
 	end if;	
	UPDATE Seans SET liczba_miejsc = wolne_miejsca_nowe WHERE id = NEW.seans_id;
	RETURN NEW;
END;
$$ language plpgsql;

CREATE TRIGGER zmniejsz_wolne_miejscaT AFTER INSERT ON Rezerwacja
FOR EACH ROW 
EXECUTE PROCEDURE zmniejsz_wolne_miejsca();


CREATE OR REPLACE FUNCTION zwieksz_wolne_miejsca() RETURNS TRIGGER AS $$
DECLARE
	wolne_miejsca_stare integer;
	wolne_miejsca_nowe integer;
BEGIN
 	SELECT liczba_miejsc FROM Seans WHERE id = OLD.seans_id INTO wolne_miejsca_stare;
 	wolne_miejsca_nowe := wolne_miejsca_stare + OLD.liczba_miejsc;
	UPDATE Seans SET liczba_miejsc = wolne_miejsca_nowe WHERE id = OLD.seans_id;
	RETURN OLD;
END;
$$ language plpgsql;

CREATE TRIGGER zwieksz_wolne_miejscaT AFTER DELETE ON Rezerwacja
FOR EACH ROW 
EXECUTE PROCEDURE zwieksz_wolne_miejsca();

INSERT INTO Film
VALUES (
        1,
        'Django',
        2015,
        'Quentin Tarantino',
        123,
        'Western',
        'USA',
        'Łowca nagród Schultz i czarnoskóry niewolnik Django wyruszają w podróż, aby odbić żonę tego drugiego z rąk bezlitosnego Calvina Candiego.',
        'https://i.etsystatic.com/18487300/r/il/df2421/1681551765/il_570xN.1681551765_4ba1.jpg'
    );

INSERT INTO Film
VALUES (
        2,
        'The Room',
        2003,
        'Tommy Wiseau',
        100,
        'Dramat',
        'USA',
        'Najlepszy-najgorszy film w historii kina.',
        'https://www.tommywiseau.com/wp-content/uploads/2016/12/wsi-imageoptim-TheRoomPosterB.jpg'
    );
    
INSERT INTO Film
VALUES (
        3,
        'La la land',
        2016,
        'Damien Chazelle',
        126,
        'Musical',
        'USA',
        'Los Angeles. Pianista jazzowy zakochuje się w początkującej aktorce.',
        'https://images-na.ssl-images-amazon.com/images/I/61pVLV%2Bz11L._AC_SY741_.jpg'
    );    

INSERT INTO Sala VALUES (1, 80);
INSERT INTO Sala VALUES (2, 100);
INSERT INTO Sala VALUES (3, 60);

INSERT INTO Seans VALUES(DEFAULT, 1, 1, 1, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 2, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 3, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 4, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 5, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 6, '20:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 1, 1, 7, '20:00', 100);

INSERT INTO Seans VALUES(DEFAULT, 2, 1, 1, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 3, 1, 2, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 2, 1, 3, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 3, 1, 4, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 2, 1, 5, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 3, 1, 6, '22:00', 100);
INSERT INTO Seans VALUES(DEFAULT, 2, 1, 7, '22:00', 100);

INSERT INTO Seans VALUES(DEFAULT, 1, 2, 1, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 2, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 3, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 4, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 5, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 6, '20:00', 80);
INSERT INTO Seans VALUES(DEFAULT, 1, 2, 7, '20:00', 80);

INSERT INTO Kasjer VALUES('Jan', 'Kowalski', 'admin', 'admin');	

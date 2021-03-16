DROP TABLE IF EXISTS Film;
DROP TABLE IF EXISTS Kasjer;
DROP TABLE IF EXISTS Klient;
DROP TABLE IF EXISTS Rezerwacja;
DROP TABLE IF EXISTS Sala;
DROP TABLE IF EXISTS Seans; 

CREATE TABLE Film (
    id int  PRIMARY KEY,
    tytuł varchar(100) NOT NULL,
    rok int NOT NULL,
    reżyser varchar(100) NOT NULL,
    czas_trwania int NOT NULL,
    gatunek varchar(100) NOT NULL,
    opis text NOT NULL,
);


CREATE TABLE Kasjer (
    id int PRIMARY KEY,
    imię varchar(20) NOT NULL,
    nazwisko varchar(20) NOT NULL,
);


CREATE TABLE Klient (
    id int PRIMARY KEY,
    imię varchar(20) NOT NULL,
    nazwisko varchar(20) NOT NULL,
);


CREATE TABLE Rezerwacja (
    id int PRIMARY KEY,
    seans_id int NOT NULL REFERENCES Seans,
    klient_id int NOT NULL REFERENCES Klient,
    kasjer_id int NOT NULL REFERENCES Kasjer,
    liczba_miejsc int NOT NULL,
    opłacono boolean NOT NULL,
);


CREATE TABLE Sala (
    numer int PRIMARY KEY,
    liczba_miejsc int NOT NULL,
);


CREATE TABLE Seans (
    id int PRIMARY KEY,
    film_id int NOT NULL REFERENCES Film,
    sala_nr int NOT NULL REFERENCES Sala,
    dzień_tygodnia varchar(10) NOT NULL CHECK (dzień_tygodnia IN ('pn', 'wt', 'śr', 'czw', 'pt', 'sb', 'ndz'),
    godzin_rozpoczęcia time NOT NULL,
    liczba_rezerwacji int NOT NULL,
);



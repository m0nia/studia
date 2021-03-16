<html>
<head>
    <title>Kino Lunar</title>
    <link href="style.css" rel="stylesheet" type="text/css"/>
</head>
<body>


<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>

<a href="repertuar.php"> <button class="button"> Repertuar </button> </a>
<a href="sprawdz_rezerwacje.php"> <button class="button"> Sprawdź rezerwację </button> </a>
<a href="logowanie.php"> <button class="button"> Panel kasjera </button> </a>

<br><br>

<?php

$seans_id = $_POST['seans_id'];
$imie = $_POST['imie'];
$nazwisko = $_POST['nazwisko'];
$email = $_POST['email'];
$ile_miejsc = $_POST['miejsca'];

$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");

// te dwa może powinny być transakcją?
// tego samego klienta przechowywać raz
$klient_qry = pg_query_params($link, "INSERT INTO Klient VALUES(DEFAULT, $1, $2, $3) RETURNING id", array($imie, $nazwisko, $email)) or die ("Could not add client\n");
$klient_id =  pg_fetch_array($klient_qry, 0);

$rezerwacja_qry = pg_query_params($link, "INSERT INTO Rezerwacja VALUES(DEFAULT, $1, $2, NULL, $3) RETURNING id", array($seans_id, 									  $klient_id["id"], $ile_miejsc)) or die("Could not add reservation");

$rezerwacja = pg_fetch_array($rezerwacja_qry, 0);

echo "<p>Rezerwacja udała się i czeka na akceptację przez kasjera. Numer rezerwacji: " . $rezerwacja["id"] . "</p>";

?>
    
</body>
</html>

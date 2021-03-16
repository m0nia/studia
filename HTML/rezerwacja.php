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

<?php
$screening_id = $_POST['seans'];
$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
$screening_qry = pg_query_params($link, "SELECT id, film_id, sala_nr, dzień_tygodnia_nazwa(dzień_tygodnia), godzina_rozpoczęcia, liczba_miejsc FROM Seans WHERE id = $1", array($screening_id));
$screening = pg_fetch_array($screening_qry, 0);
$movie_qry = pg_query_params($link, "SELECT * FROM Film WHERE id = $1", array($screening["film_id"]));
$movie = pg_fetch_array($movie_qry, 0);

				
echo "<div class='info'>
	<p>Rezerwujesz bilety na film <a href=film.php?film_id=" . $movie["id"] . ">" . $movie["tytuł"] . "</a></p>
	<p>Dzień tygodnia: " . $screening["dzień_tygodnia_nazwa"] . "</p>
	<p>Godzina: " . date_format(date_create($screening["godzina_rozpoczęcia"]), 'H:i')  . "</p>
	<p>Sala: " . $screening["sala_nr"] . "</p>
	<p>Liczba wolnych miejsc: " . $screening["liczba_miejsc"] . "</p></div>";
?>
<br>

<form action="rezerwacja2.php" method="post">
	<input type=hidden name=seans_id value= <?php echo $screening_id ?>>
  <p>
    <label for="imie">Imię:</label>
    <input id="imie" name="imie" type="text" required>
  </p>
  <p>
    <label for="nazwisko">Nazwisko:</label>
    <input id="nazwisko" name="nazwisko" type="text" required>
  </p>
  <p>
    <label for="email">Adres email:</label>
    <input id="email" name="email" type="email" required>
  </p>
    <p>
    <label for="miejsca">Liczba miejsc:</label>
    <input id="miejsca" name="miejsca" type="number" value=1 min=1 max=<?php echo $screening["liczba_miejsc"] ?> required>
    <button class='button button2' type='submit' value='rezerwuj'>Rezerwuj</button> <br>
  </p>

</form>


    
</body>
</html>

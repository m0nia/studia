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

<br>
<form action='sprawdz_rezerwacje.php' method='post'>
  <p>
    <label for="nr">Numer rezerwacji:</label>
    <input id="nr" name="nr" type="number" required>
  </p>
  <button class='button button2' type='submit'>Sprawdź</button> <br>
</form>

<?php
    if(isset($_POST["nr"])){
		$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
    	$result_rez = pg_query_params($link, "SELECT seans_id, kasjer, liczba_miejsc FROM Rezerwacja WHERE id = $1", 					array($_POST["nr"]));
    	if(pg_num_rows($result_rez) == 0){
    		echo "<p>Nie ma takiej rezerwacji lub została anulowana</p>";
    	} else {
    		$row_rez = pg_fetch_array($result_rez, 0);
    		if($row_rez["kasjer"] == NULL)
				echo "<p><B>Twoja rezerwacja czeka na potwierdzenie.</b></p>";
			else
				echo "<p><b>Twoja rezerwacja została zaakceptowana. Miłego seansu :)</b></p>";	
    	
    		$result_info = pg_query_params($link, "SELECT tytuł, dzień_tygodnia_nazwa(dzień_tygodnia), godzina_rozpoczęcia, sala_nr FROM Seans JOIN Film ON Seans.film_id = Film.id WHERE Seans.id = $1", array($row_rez["seans_id"]));
    		$row_info = pg_fetch_array($result_info, 0);
    		
    		echo "<p>Szczegóły rezerwacji:</p>";
    		echo "<p>Dzień tygodnia: " . $row_info["dzień_tygodnia_nazwa"] . "</p>
    			  <p>Godzina: " . date_format(date_create($row_info["godzina_rozpoczęcia"]), 'H:i') . "</p>
    			  <p>Sala: " . $row_info["sala_nr"] . "</p>
    			  <p>Liczba miejsc: " . $row_rez["liczba_miejsc"] . "</p>";
    	}
    }    
?>
    
</body>
</html>

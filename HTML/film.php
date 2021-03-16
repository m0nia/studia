<html>
<head>
    <title>Kino Lunar</title>
	<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>

<?php
$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
$film_id = $_GET["film_id"];
$film_info_qry = pg_query_params($link, "SELECT * FROM Film WHERE id = $1", array($film_id));
$film_info = pg_fetch_array($film_info_qry ,0);

$kiedy_grany = pg_query_params($link, "SELECT id, film_id, sala_nr, dzień_tygodnia_nazwa(dzień_tygodnia), godzina_rozpoczęcia, liczba_miejsc FROM Seans WHERE film_id = $1 ORDER BY dzień_tygodnia", array($film_id));
$numrows = pg_numrows($kiedy_grany);
?>

<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>


<a href="repertuar.php"> <button class="button"> Repertuar </button> </a>
<a href="sprawdz_rezerwacje.php"> <button class="button"> Sprawdź rezerwację </button> </a>
<a href="logowanie.php"> <button class="button"> Panel kasjera </button> </a>


<?php
	echo "<p><img src='" . $film_info["plakat"] . "'></p>
		 <p><b>Tytuł: </b>" . $film_info["tytuł"] . "</p>
		 <p><b>Rok: </b>" . $film_info["rok"] . "</p>
		 <p><b>Kraj: </b>" . $film_info["kraj"] . "</p>
		 <p><b>Reżyser: </b>" . $film_info["reżyser"] . "</p>
		 <p><b>Gatunek: </b>" . $film_info["gatunek"] . "</p>
		 <p><b>Czas trwania: </b>" . $film_info["czas_trwania"] . " minut</p>
		 <p><b>Opis: </b>" . $film_info["opis"] . "</p>";		 
?>


<h3> Kiedy gramy ten film: </h3>

<table>
<tr>
 <th>Dzień tygodnia</th>
 <th>Godzina</th>
 <th>Sala</th>
 <th>Bilety</th>
</tr>
    <?php
    
        // Loop on rows in the result set.    
    
 	for($ri = 0; $ri < $numrows; $ri++) {
	  echo "<tr>\n";
  	  $row = pg_fetch_array($kiedy_grany, $ri);
  	  echo " <td>" . $row["dzień_tygodnia_nazwa"] . "</td>
	 <td>" . date_format(date_create($row["godzina_rozpoczęcia"]), 'H:i') . "</td>
  	 <td>" . $row["sala_nr"] . "</td> <td>";
  	 if($row["liczba_miejsc"] == 0){
  	 	echo "Wyprzedane";
  	 } else {
  	 	echo "<form action='rezerwacja.php' method='post'>  	 			
              <button class='button button2' type='submit' name='seans' value=" . $row["id"] . ">Rezerwuj</button> 
              </form>";
     }
	 echo "</td></tr>";
 	}
 	pg_close($link);
	?>
    
</body>
</html>

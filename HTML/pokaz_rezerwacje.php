<?php
session_start();

if(!isset($_SESSION['login'])){
	header('Location: logowanie.php');
} else {
	$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
	if(isset($_POST['anuluj'])){
		$r_id = $_POST['anuluj'];
		$result = pg_query_params($link, "DELETE FROM Rezerwacja WHERE id = $1", array($r_id));
		if($result == false) echo "coś sie nie udaloooo";
	} elseif($_POST['zatwierdz']){
		$r_id = $_POST['zatwierdz'];
		$result = pg_query_params($link, "UPDATE Rezerwacja SET kasjer = $1 WHERE id = $2", array($_SESSION['login'], $r_id));
	}
}
?>

<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
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
		$result = pg_query($link, "SELECT imię, nazwisko, email, kasjer, liczba_miejsc, seans_id, Rezerwacja.id as r_id FROM Klient JOIN Rezerwacja ON Rezerwacja.klient_id = Klient.id");
		$numrows = pg_numrows($result);
		echo "<table>
				<tr>
				 <th>Imię</th>
				 <th>Nazwisko</th>
				 <th>Email</th>
				 <th>Liczba miejsc</th>
				 <th>Dzień tygodnia</th>
				 <th>Godzina</th>
				 <th>Sala</th>
				  <th>Status</th>
				</tr>";
		for($ri = 0; $ri < $numrows; $ri++) {
	  		echo "<tr>\n";
  	 	    $row = pg_fetch_array($result, $ri);
  	 	      	  echo "<td>" . $row["imię"] . "</td>
	 			  <td>" . $row["nazwisko"] . "</td>
 	 			  <td>" . $row["email"] . "</td>
 	 			  <td>" . $row["liczba_miejsc"] . "</td>";
 	 			  $result2 = pg_query_params($link, "SELECT tytuł, film_id, dzień_tygodnia_nazwa(dzień_tygodnia), godzina_rozpoczęcia, sala_nr FROM Seans 									JOIN Film ON Seans.film_id = Film.id WHERE Seans.id = $1", array($row["seans_id"]));
	 			 $row2 = pg_fetch_array($result2, 0);
 	 			 echo "<td>" . $row2["dzień_tygodnia_nazwa"] . "</td>
 	 			 	   <td>" . date_format(date_create($row2["godzina_rozpoczęcia"]), 'H:i') . "</td>
 	 			 	   <td>" . $row2["sala_nr"] . "</td><td>";			
  	 			  if($row["kasjer"] != NULL){
			  	 	echo "Zatwierdzona";
  	 			  } else {
  	 	            echo "<form action='pokaz_rezerwacje.php' method='post'>  	 			
                        <button class='button button2' type='submit' name='anuluj' value=" . $row["r_id"] . ">Anuluj</button>
                        <button class='button button2' type='submit' name='zatwierdz' value=" . $row["r_id"] . ">Zatwierdź</button> 
                        </form>";
    				}
	 			 echo "</td></tr>";
 		}
 		pg_close($link);

  ?>
  </body>
</html>

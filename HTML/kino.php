<?php

$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
$result = pg_query($link, "SELECT DISTINCT tytuł, gatunek, reżyser, rok, kraj, film_id FROM Seans LEFT JOIN Film ON Seans.film_id = Film.id");
$numrows = pg_numrows($result);
?>

<html>
<head>
    <title>Kino Lunar</title>
	<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>

<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>


<a href="repertuar.php"> <button class="button"> Repertuar </button> </a>
<a href="sprawdz_rezerwacje.php"> <button class="button"> Sprawdź rezerwację </button> </a>
<a href="logowanie.php"> <button class="button"> Panel kasjera </button> </a>

<h2 align=center>Gramy w tym tygodniu:</h2>

<table>
<tr>
 <th>Tytuł</th>
 <th>Gatunek</th>
 <th>Reżyser</th>
 <th>Kraj</th>
 <th>Rok</th>
 <th>Szczegóły</th>
</tr>
    <?php
    
        // Loop on rows in the result set.    
    
 	for($ri = 0; $ri < $numrows; $ri++) {
	  echo "<tr>\n";
  	  $row = pg_fetch_array($result, $ri);
  	  echo " <td>" . $row["tytuł"] . "</td>
	 <td>" . $row["gatunek"] . "</td>
 	 <td>" . $row["reżyser"] . "</td>
 	 <td>" . $row["kraj"] . "</td>
 	 <td>" . $row["rok"] . "</td>
 	 <td> <form action='film.php' method='get'>
 	 	  <button class='button button2' type='submit' name='film_id' value=" . $row["film_id"] . ">Zobacz</button>
 	 	  </form></td>
	 </tr>
	 ";
 	}
 	pg_close($link);
	?>
    
</body>
</html>

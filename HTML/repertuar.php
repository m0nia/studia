<html>
<head>
    <title>Kino Lunar</title>
	<link href="style.css" rel="stylesheet" type="text/css"/>
</head>
<body>


<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>


<form action="repertuar.php" method=POST>
<button class='button' type='submit' name='day' value='1'>Poniedziałek</button>
<button class='button' type='submit' name='day' value='2'>Wtorek</button>
<button class='button' type='submit' name='day' value='3'>Środa</button>
<button class='button' type='submit' name='day' value='4'>Czwartek</button>
<button class='button' type='submit' name='day' value='5'>Piątek</button>
<button class='button' type='submit' name='day' value='6'>Sobota</button>
<button class='button' type='submit' name='day' value='7'>Niedziela</button>
</form>

<?php
$day = (!empty($_POST['day'])) ? $_POST['day'] : 1;
$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
$result = pg_query_params($link, 
  "SELECT tytuł, rok, reżyser, gatunek, godzina_rozpoczęcia, czas_trwania, sala_nr, liczba_miejsc, Seans.id AS seans_id
  FROM Film JOIN Seans ON Film.id = Seans.film_id
  WHERE dzień_tygodnia = $1
  ORDER BY godzina_rozpoczęcia;",
  array($day)
);
$numrows = pg_numrows($result);
?>


<table>
<tr>
 <th>Tytuł</th>
 <th>Rok</th>
 <th>Reżyser</th>
 <th>Gatunek</th>
 <th>Godzina</th>
 <th>Czas trwania</th>
 <th>Sala</th>
 <th>Bilety</th>
</tr>
    <?php
    
        // Loop on rows in the result set.    
    
 	for($ri = 0; $ri < $numrows; $ri++) {
	  echo "<tr>\n";
  	  $row = pg_fetch_array($result, $ri);
  	  echo " <td>" . $row["tytuł"] . "</td>
	 <td>" . $row["rok"] . "</td>
 	 <td>" . $row["reżyser"] . "</td>
 	 <td>" . $row["gatunek"] . "</td>
 	 <td>" . date_format(date_create($row["godzina_rozpoczęcia"]), 'H:i') . "</td>
   	 <td>" . $row["czas_trwania"] . "</td>
  	 <td>" . $row["sala_nr"] . "</td> <td>";
  	 if($row["liczba_miejsc"] == 0){
  	 	echo "Wyprzedane";
  	 } else {
  	 	echo "<form action='rezerwacja.php' method='post'>  	 			
              <button class='button button2' type='submit' name='seans' value=" . $row["seans_id"] . ">Rezerwuj</button> 
              </form>";
     }
	 echo "</td></tr>";
 	}
 	pg_close($link);
	?>
    
</body>
</html>

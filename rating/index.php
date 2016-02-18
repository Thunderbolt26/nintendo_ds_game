<?php

   for($j = 0; $j < 4;$j++)
   {
     switch($j)
     {
       case 0:
       $file = "ratingcolor.txt";
       $namerating="Color rating";
       break;
       case 1:
       $file = "ratingbasket.txt";
       $namerating="Basket rating";
       break;
       case 2:
       $file = "ratingfootball.txt";
       $namerating="Football rating";
       break;
       case 3:
       $file = "mainrating.txt";
       $namerating="Main rating";
       break;
     }
     $current = file_get_contents($file);
     $rating=explode(PHP_EOL,$current);

     echo '<table border="1" cellspacing="0" cellpadding="15" width="90%" height="130">';
     echo "<caption> <h1>";
     echo $namerating;
     if(count($rating)==1)
     {
        echo " doesn't have records";
        echo "</h1></caption>";
        continue;
     }
     echo "</h1></caption>";
     echo "<tr>";
     echo '<td width="5%" style="background-color:#bfffdc"><b> ¹ </b></td>';
     echo '<td style="background-color:#afffdc">User</td>';
     echo '<td style="background-color:#cfffdc">Score</td>';
     echo "</tr>";



     for ($i = 0; $i < count($rating)-1; $i++)
     {
        echo "<tr>";
        echo '<td><b>';
        echo $i+1;
        echo '</b></td>';

        $string = explode(" ",$rating[$i]);

        echo '<td><b>';
		echo $string[0];
        echo '</b></td>';

        echo '<td><b>';
		echo $string[1];
        echo '</b></td>';
        echo "</tr>";
     }
   }
?>

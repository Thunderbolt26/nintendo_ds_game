<?php
 if (!empty($_GET["username"]))//&&!empty($_GET["score0"])&&!empty($_GET["score1"])&&!empty($_GET["score2"]))&&!empty($_GET["scoreColor"])!empty($_GET["scoreBasketball"])&&!empty($_GET["scoreFootball"]))
 {
   for($j = 0; $j < 4;$j++)
   {
     switch($j)
     {
       case 0:
       $file = "ratingcolor.txt";

       if(!empty($_GET["score0"]))
       {
         $scorestr = $_GET["score0"];
         $score=(int)$_GET["score0"];
       }
       else
       {
         $scorestr = ""+0;
         $score=0;
       }
       $namerating="color rating";
       break;
       case 1:
       $file = "ratingbasket.txt";
       if(!empty($_GET["score1"]))
	   {
	      $scorestr = $_GET["score1"];
	      $score=(int)$_GET["score1"];
	   }
	   else
	   {
	      $scorestr =""+0;
	      $score=0;
       }
       $namerating="basket rating";
       break;
       case 2:
       $file = "ratingfootball.txt";
       if(!empty($_GET["score2"]))
	   {
	      $scorestr = $_GET["score2"];
	      $score=(int)$_GET["score2"];
	   }
	   else
	   {
	      $scorestr =""+0;
	      $score=0;
       }
       $namerating="football rating";
       break;
       case 3:
       $file = "mainrating.txt";
       if(!empty($_GET["score0"]))$sc0=(int)$_GET["score0"]; else $sc0=0;
       if(!empty($_GET["score1"]))$sc1=(int)$_GET["score1"]; else $sc1=0;
       if(!empty($_GET["score2"]))$sc2=(int)$_GET["score2"]; else $sc2=0;
       $score=$sc0 + $sc1 + $sc2;
       $scorestr = "" + $score;
       $namerating="main rating";
       break;
     }
     $current = file_get_contents($file);
     $flag=0;
     $old_pos=0;
     $new_pos=0;
     $string="";
     $rating=explode(PHP_EOL,$current);
     $pattern = '/.+ (\d+)/';
     $replacement = '$1';

     if(count($rating)==1)
     {
       $string .= $_GET["username"] . " " . $scorestr . PHP_EOL;
       $flag=1;
       $new_pos=1;
     }
     else
     for ($i = 0; $i < 6-$flag + $old_pos and $i <= count($rating)-$flag + $old_pos; $i++)
     {
       $iscore = (int)preg_replace($pattern, $replacement, $rating[$i]);

       if(($iscore < $score) && ($flag==0)&&($old_pos==0))
       {
          $string .=  $_GET["username"] . " " . $scorestr . PHP_EOL;
          $flag=1;
          $new_pos=$i+1;
          if($i == 6-$flag)
           break;
       }
       $pos = strpos($rating[$i], $_GET["username"]);
       if ($pos !== false)
	   {
	       $old_pos=1;
	       if($flag==1)
	       {
	         if($i == count($rating)-$flag)
               break;
	         continue;
	       }
       }
       if($rating[$i]!="")
       $string .= $rating[$i] . PHP_EOL;
   }
   if(count($rating)<6)
   {
        if($flag==0 &&($old_pos==0))
	    {
		   $string .=  $_GET["username"] . " " . $scorestr . PHP_EOL;
		   $flag=1;
		   $new_pos=count($rating);
        }
   }
   if($flag == 1)
     echo "Cool, you are " . $new_pos . " in ";
   else if($old_pos==1)
     echo "Sorry, you don't break your record in ";
   else
     echo "Sorry, you aren't in ";
   echo $namerating;
   echo "\n";
   file_put_contents($file, $string);
   }
 }
 else
 {
   echo "You don't have name";
 }

?>

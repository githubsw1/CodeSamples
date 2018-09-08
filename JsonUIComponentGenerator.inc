<?php
/*
 * Description: This class is use to populate a list of network sites
*/

class JsonUIComponentGenerator
{
	function init()
	{
		include 'config.inc';
	}
	
	function buildDropDownList()
	{
		try
		{
			echo "Begin Creating Drown Down Menu List\n";
				
			$prop = Config::getConfig();
	
			$filename = $prop['sites']; // Get data from properties file
	
			$file = fopen($filename , "r");
	
			$data = array();
	
			// read file and store results in array for parsing
			
			while(! feof( $file ))
			{
				$line = fgets($file);
	
				array_push($data , $line);
			}
	
			fclose($file);
	
			$list = $prop['export']; // get header names from properties file to be populated in the drop menu list
	
			$items = str_getcsv( $list );
	
			$header = array();
	
			foreach($data as $row)
			{
				$header = str_getcsv( $row , "\t");
	
				break;
			}
	
			$skip = 0;
	
			foreach($items as $column)
			{
				if(count(array_keys($header,$column)) > 1)
				{
					$key = array_keys($header,$column);
	
					$itemlist = array();
	
					foreach($key as $k)
					{
						foreach($data as $row)
						{
							if($skip == 0)
							{
								$skip = 1;
	
								continue;
							}
							 
							$line = str_getcsv( $row , "\t");
							 
							if(isset($line[$k]) && trim($line[$k]) != '' && strlen(trim($line[$k])) > 0 && $column != $line[$k])
							{
								$sites = preg_split("/,/",$line[$k]);
	
								foreach ($sites as $site)
								{
									if(!in_array($site, $itemlist))
									{
										if(!empty($site) && trim(strlen($site)) > 0)
										{
											array_push($itemlist , "$site");
										}
									}
								}
							}
							 
						}
					}
	
					if(preg_match("/\*/",$column))
					{
						$column = str_replace("*", "", $column);
					}
	
					$file = $column . ".json";
	
					$path = $prop['apphome'];
	
					$path .= $file;
	
					$filename = $path;
				  
					if(!file_exists($filename))
					{
						$items = array_unique( $itemlist );
	
						$d = preg_split("/\n/",implode("\n",$items));
						 
						sort($d);
						 
						$len = count($d);
						 
						$index = 1;
						 
						$str = array();

						// build list in json format the UI component can read to populate drop menu
						
						for($x=0; $x < $len; $x++)
						{
							if($x == 0)
							{
								$str[$x] = array("id" => $index, "text" => "$d[$x]", "selected" => true);
							}
							else
							{
								$str[$x] = array("id" => $index, "text" => "$d[$x]");
							}
		
							$index++;
						}
		
						file_put_contents( $filename , json_encode($str) );
					}
				}
			}	
			
			echo "End Drown Down Menu List\n";
		}	
		catch(Exception $e)
		{
				die($e->getMessage());
		}
	}					
}

?>
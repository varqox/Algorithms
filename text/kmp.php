<?php

/**
 * Implementation of the Knuth-Morris-Pratt Algorithm
 *
 * @copyright   Copyright (c) 2013, Maciej Czyżewski <pozer.maciek@gmail.com>
 * @license     MIT
 */

class KMP
{
	public function usage($argv)
	{
	    return 'usage: ' . $argv[0] . ' <Pattern> [Text]' . PHP_EOL;
	}

	public function match($T, $P)
	{
		$array = array();

	    $n = strlen($T);
	    $m = strlen($P);
	    $pi = self::compute_prefix($P);

	    $q = 0;
	    $l = 0;

	    for ($i = 0; $i < $n; $i++) {
	        while ($q > 0 && $P[$q] != $T[$i]) {
	            $q = $pi[$q];
	        }

	        if ($P[$q] == $T[$i]) {
	            $q = $q + 1;
	        }

	        if ($q == $m) {
	        	array_push($array, $i - $m + 1);

	            $l = $i;
	            $q = $pi[$q];
	        }
	    }

	    return $array;
	}

	public function compute_prefix($P)
	{
	    $m = strlen($P);
	    $pi = array();
	    $pi[1] = 0;
	    $k = 0;

	    for ($q = 1; $q < $m; $q++) {
	        while ($k > 0 && $P[$k] != $P[$q]) {
	            $k = $pi[$k];
	        }
	        if ($P[$k] == $P[$q]) {
	            $k = $k + 1;
	        }
	        $pi[$q+1] = $k;
	    }

	    return $pi;
	}
}

/**
 * Testing via command line.
 */

if (isset($argv[1])) { $P = $argv[1]; }
if (isset($argv[2])) { $T = $argv[2]; }

if (isset($P)) {
    if (isset($T)) {
        $response = KMP::match($T, $P);
    } else {
        $response = KMP::compute_prefix($P);
    }

    var_dump($response);
} else {
    echo KMP::usage($argv);
}

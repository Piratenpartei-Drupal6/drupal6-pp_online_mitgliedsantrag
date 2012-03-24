<?php

	function pp_online_mitgliedsantrag_get_nationalitaeten() {
		$pp_online_mitgliedsantrag_nationalitaeten = array(
			"BE"	=>	"Belgisch",
			"BG"	=>	"Bulgarisch",
			"DK"	=>	"Dänisch",
			"DE"	=>	"Deutsch",
			"EE"	=>	"Estnisch",
			"FI"	=>	"Finnisch",
			"FR"	=>	"Französisch",
			"GR"	=>	"Griechisch",
			"IE"	=>	"Irisch",
			"IT"	=>	"Italienisch",
			"LV"	=>	"Lettisch",
			"LT"	=>	"Litauisch",
			"LU"	=>	"Luxemburgisch",
			"MT"	=>	"Maltesisch",
			"NL"	=>	"Holländisch",
			"AT"	=>	"Österreichisch",
			"PL"	=>	"Polnisch",
			"PT"	=>	"Portugiesisch",
			"RO"	=>	"Rumänisch",
			"SE"	=>	"Schwedisch",
			"SK"	=>	"Slowakisch",
			"SI"	=>	"Slowenisch",
			"ES"	=>	"Spanisch",
			"CZ"	=>	"Tschechisch",
			"HU"	=>	"Ungarisch",
			"GB"	=>	"Britisch",
			"CY"	=>	"Zypriotisch",
			"CH"	=>	"Schweizerisch",
			"TR"	=>	"Türkisch",
		);

		uasort($pp_online_mitgliedsantrag_nationalitaeten, 'pp_online_mitgliedsantrag_asort_with_umlauts');

		return $pp_online_mitgliedsantrag_nationalitaeten;
	}

	function pp_online_mitgliedsantrag_asort_with_umlauts($s1, $s2) {
		$search = array('Ä','Ö','Ü','ß');
		$replace = array('A','O','U','s');

		return strcmp(
			str_ireplace($search, $replace, $s1),
			str_ireplace($search, $replace, $s2)
		);
	}

?>

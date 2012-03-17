<?php

/**
 * Admin menu page for the basic settings
 */
function pp_online_mitgliedsantrag_admin_settings_form() {
	// gpg
		if (!pp_online_mitgliedsantrag_init_gpg())
			return;
		$gpg = pp_online_mitgliedsantrag_get_gpg();

	// get keys
		$data = pp_online_mitgliedsantrag_get_gpg_keys($gpg);

	// stats
		$sql = "SELECT COUNT(*) AS `count` FROM {pp_online_mitgliedsantrag}";
		$results = db_query($sql, $code);
		$stats = db_fetch_array($results);
		#print_r($stats);
		switch ($stats["count"]) {
			case 0:
				$befindet = "befinden";
				$anzahl = "keine";
				$antrag = "Anträge";
				break;
			case 1:
				$befindet = "befindet";
				$anzahl = "ein";
				$antrag = "Antrag";
				break;
			default:
				$befindet = "befinden";
				$anzahl = $stats["count"];
				$antrag = "Anträge";
				break;
		}

	// form
		$form = array();

		$form['stats'] = array(
			'#type' => 'fieldset',
			'#title' => t('Statistik'),
			'#description' => t("Es @befindet sich <b>@anzahl @antrag</b> in der Datenbank.", array("@befindet" => $befindet, "@anzahl" => $anzahl, "@antrag" => $antrag)),
		);

		$form['stats']['pp_online_mitgliedsantrag_delete'] = array(
			'#type' => 'textfield',
			'#title' => t('Lösch-Intervall'),
			'#description' => "Angabe in Tagen.",
			'#required' => true,
			'#default_value' => variable_get('pp_online_mitgliedsantrag_delete', PP_ONLINE_MITGLIEDSANTRAG_DELETE_INTERVAL),
			'#size' => 2,
/*
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_width_100',
			),
*/
		);

		$form['key'] = array(
			'#type' => 'fieldset',
			'#title' => t('Schlüssel-Einstellungen'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_settings_fieldset',
			),
		);

		$sender = array();
		foreach($data["private"] as $k => $v) {
			$sender[$v["keyid"]] = $v["email"]." (0x".$v["keyid"].")";
		}
		asort($sender);
		if (!empty($sender)) {
			$form['key']['pp_online_mitgliedsantrag_sender'] = array(
				'#type' => 'select',
				'#title' => t('Absender'),
				'#default_value' => variable_get('pp_online_mitgliedsantrag_sender', NULL),
				'#options' => $sender,
			);

			/* M!tgli3d */
			$form['key']['pp_online_mitgliedsantrag_password'] = array(
				'#type' => 'textfield',
				'#title' => t('Passwort'),
				'#description' => t("Passwort des ausgewählten Schlüssels."),
				'#default_value' => variable_get('pp_online_mitgliedsantrag_password', NULL),
			);
		} else {
			$form['key']['pp_online_mitgliedsantrag_sender_note'] = array(
				'#type' => 'item',
				'#title' => t('Absender'),
				'#description' => t('Bitte erst einen privaten <a href="membership/pgp">PGP-Schlüssel</a> hinzufügen.'),
			);
		}

		$receiver = array();
		foreach($data["public"] as $k => $v) {
			$receiver[$v["keyid"]] = $v["email"]." (0x".$v["keyid"].")";
		}
		asort($receiver);
		if (!empty($sender)) {
			$form['key']['pp_online_mitgliedsantrag_receiver'] = array(
				'#type' => 'select',
				'#title' => t('Empfänger'),
				'#default_value' => variable_get('pp_online_mitgliedsantrag_receiver', NULL),
				'#options' => $receiver,
			);
		} else {
			$form['key']['pp_online_mitgliedsantrag_receiver_note'] = array(
				'#type' => 'item',
				'#title' => t('Empfänger'),
				'#description' => t('Bitte erst öffentliche <a href="membership/pgp">PGP-Schlüssel</a> hinzufügen.'),
			);
		}


		$form['standard'] = array(
			'#type' => 'fieldset',
			'#title' => t('Standard-Einstellungen'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_settings_fieldset',
			),
		);

		$form['standard']['pp_online_mitgliedsantrag_force_ssl'] = array(
			'#type' => 'checkbox',
			'#title' => t('SSL-Verschlüsselung erzwingen'),
			'#default_value' => variable_get('pp_online_mitgliedsantrag_force_ssl', NULL),
		);

		$form['standard']['pp_online_mitgliedsantrag_wiki_link'] = array(
			'#type' => 'textfield',
			'#title' => t('Verfahrens-Wiki-Link'),
			'#default_value' => variable_get('pp_online_mitgliedsantrag_wiki_link', NULL),
		);


		$form['state'] = array(
			'#type' => 'fieldset',
			'#title' => t('Bundesland'),
		);
		$form['state']['pp_online_mitgliedsantrag_state'] = array(
			'#type' => 'select',
			'#title' => t('Ausgewähltes Bundesland'),
			'#default_value' => variable_get('pp_online_mitgliedsantrag_state', NULL),
			'#options' => pp_opengeodb_api_getStates(),
		);


		$form['district'] = array(
			'#type' => 'fieldset',
			'#title' => t('Kreise und kreisfreie Städte'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_settings_county_fieldset',
			),
		);
		$state = variable_get('pp_online_mitgliedsantrag_state', NULL);
		if (empty($state)) {
			$form['district']['note'] = array(
				'#type' => 'item',
				'#description' => t('Bitte erst ein Bundesland ausgewählen.'),
			);
		} else {
			$counties = pp_opengeodb_api_getCountiesByState($state);

			$form['district']['#title'] .= " (".sizeof($counties).")";

			$receiver = array();
			foreach($data["public"] as $k => $v) {
				$receiver[$v["keyid"]] = $v["email"]." (0x".$v["keyid"].")";
			}
			asort($receiver);
			$receiver = array_merge(array("" => "Kein Key"), $receiver);
			foreach ($counties as $county) {
				$name = 'pp_online_mitgliedsantrag_county_'.strtolower($state).'_'.strtolower($county);
				$form['district'][$name] = array(
					'#type' => 'textfield',
					'#default_value' => $county,
					'#disabled' => true,
				);

				$name = 'pp_online_mitgliedsantrag_receiver_'.strtolower($state).'_'.strtolower($county);
				$form['district'][$name] = array(
					'#type' => 'select',
					'#default_value' => variable_get($name, NULL),
					'#options' => $receiver,
				);
			}
		}


	// return
		return system_settings_form($form);
}

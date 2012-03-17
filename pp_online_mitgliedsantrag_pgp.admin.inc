<?php

/**
 * Admin menu page for the basic settings
 */
function pp_online_mitgliedsantrag_admin_pgp_form() {
	// gpg
		if (!pp_online_mitgliedsantrag_init_gpg())
			return;
		$gpg = pp_online_mitgliedsantrag_get_gpg();

	// get keys
		$data = pp_online_mitgliedsantrag_get_gpg_keys($gpg);

	// check del
		if (isset($_GET["del"])) {
			foreach($data["private"] as $k => $v) {
				if ($v["keyid"] == $_GET["del"]) {
					unset($data["private"][$k]);
					$gpg->deletePrivateKey($v["keyid"]);
					drupal_set_message("Einen privaten PGP-Schlüssel gelöscht.");
				}
			}
			foreach($data["public"] as $k => $v) {
				if ($v["keyid"] == $_GET["del"]) {
					unset($data["public"][$k]);
					$gpg->deletePublicKey($v["keyid"]);
					drupal_set_message("Einen öffentlichen PGP-Schlüssel gelöscht.");
				}
			}
			foreach($data["invalid"] as $k => $v) {
				if ($v["keyid"] == $_GET["del"]) {
					unset($data["invalid"][$k]);
					$gpg->deletePublicKey($v["keyid"]);
					drupal_set_message("Einen öffentlichen PGP-Schlüssel gelöscht.");
				}
			}
		}


	// form
		$form = array();

		$form['private_keys'] = array(
			'#type' => 'fieldset',
			'#title' => t('Private Schlüssel'),
			'#description' => t('Eine Auflistung aller installierten privaten PGP-Schlüssel.'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_pgp_fieldset',
			),
			'#collapsible' => true,
			'#collapsed' => false,
		);

		foreach($data["private"] as $k => $v) {
			$form['private_keys'][$k]["email"] = array(
				'#type' => 'textfield',
				'#title' => t('E-Mail'),
				'#value' => $v["email"],
				'#disabled' => true,
			);
			$form['private_keys'][$k]["name"] = array(
				'#type' => 'textfield',
				'#title' => t('Name'),
				'#value' => $v["name"],
				'#disabled' => true,
			);
			$form['private_keys'][$k]["keyid"] = array(
				'#type' => 'textfield',
				'#title' => t('Key-ID'),
				'#value' => "0x".$v["keyid"],
				'#disabled' => true,
				'#attributes' => array(
					'class' => 'pp_online_mitgliedsantrag_admin_pgp_keyid',
				),
			);
			$form['private_keys'][$k]["delete"] = array(
				'#type' => 'item',
				'#value' => '<a href="#">'.t('Löschen').'</a>',
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-delete-wrapper" rel="'.$v["keyid"].'">',
				'#suffix' => '</div>',
			);
			$form['private_keys'][$k]["expirationdate"] = array(
				'#type' => 'item',
				'#value' => showExpirationDate($v),
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-expirationdate-wrapper">',
				'#suffix' => '</div>',
			);
		}


		$form['public_keys'] = array(
			'#type' => 'fieldset',
			'#title' => t('Öffentliche Schlüssel'),
			'#description' => t('Eine Auflistung aller installierten öffentlichen PGP-Schlüssel.'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_pgp_fieldset',
			),
			'#collapsible' => true,
			'#collapsed' => false,
		);

		foreach($data["public"] as $k => $v) {
			$form['public_keys'][$k]["email"] = array(
				'#type' => 'textfield',
				'#title' => t('E-Mail'),
				'#value' => $v["email"],
				'#disabled' => true,
			);
			$form['public_keys'][$k]["name"] = array(
				'#type' => 'textfield',
				'#title' => t('Name'),
				'#value' => $v["name"],
				'#disabled' => true,
			);
			$form['public_keys'][$k]["keyid"] = array(
				'#type' => 'textfield',
				'#title' => t('Key-ID'),
				'#value' => "0x".$v["keyid"],
				'#disabled' => true,
				'#attributes' => array(
					'class' => 'pp_online_mitgliedsantrag_admin_pgp_keyid',
				),
			);
			$form['public_keys'][$k]["delete"] = array(
				'#type' => 'item',
				'#value' => '<a href="#">'.t('Löschen').'</a>',
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-delete-wrapper" rel="'.$v["keyid"].'">',
				'#suffix' => '</div>',
			);
			$form['public_keys'][$k]["expirationdate"] = array(
				'#type' => 'item',
				'#value' => showExpirationDate($v),
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-expirationdate-wrapper">',
				'#suffix' => '</div>',
			);
		}


		$form['expired_keys'] = array(
			'#type' => 'fieldset',
			'#title' => t('Abgelaufene Schlüssel'),
			'#description' => t('Eine Auflistung aller abgelaufener PGP-Schlüssel.'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_pgp_fieldset',
			),
			'#collapsible' => true,
			'#collapsed' => false,
		);

		foreach($data["expired"] as $k => $v) {
			$form['expired_keys'][$k]["email"] = array(
				'#type' => 'textfield',
				'#title' => t('E-Mail'),
				'#value' => $v["email"],
				'#disabled' => true,
			);
			$form['expired_keys'][$k]["name"] = array(
				'#type' => 'textfield',
				'#title' => t('Name'),
				'#value' => $v["name"],
				'#disabled' => true,
			);
			$form['expired_keys'][$k]["keyid"] = array(
				'#type' => 'textfield',
				'#title' => t('Key-ID'),
				'#value' => "0x".$v["keyid"],
				'#disabled' => true,
				'#attributes' => array(
					'class' => 'pp_online_mitgliedsantrag_admin_pgp_keyid',
				),
			);
			$form['expired_keys'][$k]["delete"] = array(
				'#type' => 'item',
				'#value' => '<a href="#">'.t('Löschen').'</a>',
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-delete-wrapper" rel="'.$v["keyid"].'">',
				'#suffix' => '</div>',
			);
			$form['expired_keys'][$k]["expirationdate"] = array(
				'#type' => 'item',
				'#value' => showExpirationDate($v),
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-expirationdate-wrapper">',
				'#suffix' => '</div>',
			);
		}


		$form['invalid_keys'] = array(
			'#type' => 'fieldset',
			'#title' => t('Zurückgezogene Schlüssel'),
			'#description' => t('Eine Auflistung aller zurückgezogener PGP-Schlüssel.'),
			'#attributes' => array(
				'class' => 'pp_online_mitgliedsantrag_admin_pgp_fieldset',
			),
			'#collapsible' => true,
			'#collapsed' => false,
		);

		foreach($data["invalid"] as $k => $v) {
			$form['invalid_keys'][$k]["email"] = array(
				'#type' => 'textfield',
				'#title' => t('E-Mail'),
				'#value' => $v["email"],
				'#disabled' => true,
			);
			$form['invalid_keys'][$k]["name"] = array(
				'#type' => 'textfield',
				'#title' => t('Name'),
				'#value' => $v["name"],
				'#disabled' => true,
			);
			$form['invalid_keys'][$k]["keyid"] = array(
				'#type' => 'textfield',
				'#title' => t('Key-ID'),
				'#value' => "0x".$v["keyid"],
				'#disabled' => true,
				'#attributes' => array(
					'class' => 'pp_online_mitgliedsantrag_admin_pgp_keyid',
				),
			);
			$form['invalid_keys'][$k]["delete"] = array(
				'#type' => 'item',
				'#value' => '<a href="#">'.t('Löschen').'</a>',
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-delete-wrapper" rel="'.$v["keyid"].'">',
				'#suffix' => '</div>',
			);
			$form['invalid_keys'][$k]["expirationdate"] = array(
				'#type' => 'item',
				'#value' => showExpirationDate($v),
				'#prefix' => '<div class="edit-pp-online-mitgliedsantrag-expirationdate-wrapper">',
				'#suffix' => '</div>',
			);
		}


		$form['add_key'] = array(
			'#type' => 'fieldset',
			'#title' => t('Neuer Schlüssel'),
			'#description' => t('Einen neuen Schlüssel einfach via Copy-Paste hinzufügen.'),
		);

		$form['add_key']['pp_online_mitgliedsantrag_add_key'] = array(
			'#type' => 'textarea',
		);

		$form['add_key']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Hinzufügen'),
		);

	// return
		return $form;
}


function pp_online_mitgliedsantrag_admin_pgp_form_validate($form, &$form_state) {
	// check pgp
		$key = $form_state['values']['pp_online_mitgliedsantrag_add_key'];
		$key = trim($key);
		if (strpos($key, "-----BEGIN PGP PUBLIC KEY BLOCK-----") === false || strpos($key, "-----END PGP PUBLIC KEY BLOCK-----") === false)
			form_set_error('pp_online_mitgliedsantrag_add_key', t('Es muss ein gültiger Schlüssel eingefügt werden.'));
}


function pp_online_mitgliedsantrag_admin_pgp_form_submit($form, &$form_state) {
	// gpg
		if (!pp_online_mitgliedsantrag_init_gpg())
			return;
		$gpg = pp_online_mitgliedsantrag_get_gpg();

	// import
		$data = $gpg->importKey($form_state['values']['pp_online_mitgliedsantrag_add_key']);
		#print_r($data);

	// result
		if ($data["public_imported"] > 0) {
			if ($data["public_imported"] == 1)
				drupal_set_message("Einen öffentlichen PGP-Schlüssel importiert.");
			else
				drupal_set_message($data["public_imported"]." öffentliche PGP-Schlüssel importiert.");
		}
		if ($data["public_unchanged"] > 0) {
			if ($data["public_unchanged"] == 1)
				drupal_set_message("Einen öffentlichen PGP-Schlüssel nicht verändert.");
			else
				drupal_set_message($data["public_unchanged"]." öffentliche PGP-Schlüssel nicht verändert.");
		}

		if ($data["private_imported"] > 0) {
			if ($data["private_imported"] == 1)
				drupal_set_message("Einen privaten PGP-Schlüssel importiert.");
			else
				drupal_set_message($data["private_imported"]." private PGP-Schlüssel importiert.");
		}
		if ($data["private_unchanged"] > 0) {
			if ($data["private_unchanged"] == 1)
				drupal_set_message("Einen privaten PGP-Schlüssel nicht verändert.");
			else
				drupal_set_message($data["private_unchanged"]." private PGP-Schlüssel nicht verändert.");
		}
}

function showExpirationDate($key) {
	if ($key["expirationdate"] == 0)
		return t("Wird nicht verfallen");
	else if ($key["expirationdate"] < time())
		return t("Ist verfallen am")." ".format_date($key["expirationdate"]);
	else
		return t("Verfällt am")." ".format_date($key["expirationdate"]);
}

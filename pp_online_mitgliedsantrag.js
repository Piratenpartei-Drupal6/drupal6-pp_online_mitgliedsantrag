var pp_online_mitgliedsantrag_required_hint = ' <span class="form-required" title="Diese Angabe wird benötigt.">*</span>';

$(document).ready(function() {
	$('#edit-pp-online-mitgliedsantrag-einzug').bind('change', function() {
		if ($(this).is(':checked')) {
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-kontoinhaber']"));
			$('#edit-pp-online-mitgliedsantrag-kontoinhaber').addClass("required");
			/* HIT-16
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-kontonummer']"));
			$('#edit-pp-online-mitgliedsantrag-kontonummer').addClass("required");
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-blz']"));
			$('#edit-pp-online-mitgliedsantrag-blz').addClass("required");/**/
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-bank']"));
			$('#edit-pp-online-mitgliedsantrag-bank').addClass("required");
			/* HIT-16 */
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-iban']"));
			$('#edit-pp-online-mitgliedsantrag-iban').addClass("required");
			$('#pp_online_bank_data fieldset').show();
		} else {
			$("label[for='edit-pp-online-mitgliedsantrag-kontoinhaber'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-kontoinhaber').removeClass("required");
			/* HIT-16
			$("label[for='edit-pp-online-mitgliedsantrag-kontonummer'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-kontonummer').removeClass("required");
			$("label[for='edit-pp-online-mitgliedsantrag-blz'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-blz').removeClass("required");/**/
			$("label[for='edit-pp-online-mitgliedsantrag-bank'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-bank').removeClass("required");
			/* HIT-16 */
			$("label[for='edit-pp-online-mitgliedsantrag-iban'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-iban').removeClass("required");
			$('#pp_online_bank_data fieldset').hide();
		}
	});

	$('#edit-pp-online-mitgliedsantrag-beitragsminderung').bind('change', function() {
		if ($(this).is(':checked')) {
			$("label[for='edit-pp-online-mitgliedsantrag-beitrag'] > span").remove();
			$('#edit-pp-online-mitgliedsantrag-beitrag').removeClass("required");
		} else {
			$(pp_online_mitgliedsantrag_required_hint).appendTo($("label[for='edit-pp-online-mitgliedsantrag-beitrag']"));
			$('#edit-pp-online-mitgliedsantrag-beitrag').addClass("required");
		}
	});

	$('#edit-pp-online-mitgliedsantrag-plz').bind('keyup', function() {
		var zipcode = $(this).val();
		if (zipcode.length >= 3) {
			$.ajax({
				type: "POST",
				url: "/ajax/membership/get/citiesbyzipcode",
				data: "zipcode="+zipcode,
				dataType: "json",
				success: function(cities) {
					var options = "";
					$.each(cities, function(index, value) {
						options += "<option value='"+index+"'>"+value+"</option>";
					});
					$('#edit-pp-online-mitgliedsantrag-ort').html(options);
				}
			});
		}
	});

	$('#edit-pp-online-mitgliedsantrag-ort').bind('change', function() {
		var city = $(this).val();
		var zipcode = $('#edit-pp-online-mitgliedsantrag-plz').val();
		$.ajax({
			type: "POST",
			url: "/ajax/membership/get/zipcodebycity",
			data: "city="+city+"&zipcode="+zipcode,
			dataType: "json",
			success: function(zipcode) {
				if (zipcode) {
					$('#edit-pp-online-mitgliedsantrag-plz').val(zipcode);
				}
			}
		});
	});

	$('div.edit-pp-online-mitgliedsantrag-delete-wrapper div.form-item a').bind('click', function() {
		var key = $(this).parent().parent().attr("rel");
		var answer = confirm('PGP-Key 0x'+key+' wirklich löschen?');
		if (answer)
			location.href = window.location.pathname+"?del="+key;
	});
});

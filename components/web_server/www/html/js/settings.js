---
---
var ssid = getE('ssid'),
	password = getE('password'),
	ent_username = getE('enterprise_username'),
	ent_identity = getE('enterprise_identity'),
	apSsid = getE('apSsid'),
	apPassword = getE('apPassword'),
	macAp = getE('macAp'),
	randMacAp = getE('randMacAp'),
	staticIP = getE('staticIP'),
	subnetMask = getE('subnetMask'),
	gateway = getE('gateway'),
	dnsIP = getE('dnsIp'),
	CustomDns = getE('CustomDns'),
	darkMode = getE('darkMode'),
	ledEnable = getE('useLed'),
	webServer = getE('webServer'),
	apIP = getE('apIP'),
	macContainer = getE('macContainer'),
	dnsIpContainer = getE('dnsIpContainer'),
	nextDnsUrlContainer = getE('nextDnsUrlContainer'),
	adminUsername = getE('adminUsername'),
	adminPassword = getE('adminPassword'),
	maxLoginAttempts = getE('maxLoginAttempts'),
	blockingTimeMin = getE('blockingTimeMin'),
	res = '',
	checkboxChanges,
	inputChanges;

/* Add listeners to checkboxes */

randMacAp.addEventListener("change", switchMAC, false);
CustomDns.addEventListener("change", dns_Ip, false);

function getData() {
	getResponse('{% if jekyll.environment == "development" %}settings.json{% else %}data/settings.json{% endif %}', function (responseText) {
		// try {
		// 	res = JSON.parse(responseText);
		// } catch (e) {
		// 	fadeIn();
		// 	notify("{% t settings.messages.E2 %} (E2)");
		// 	return;
		// }
		try {
			res = JSON.parse(responseText);
			log("RESPONSE  ~ ", res, true)
		} catch (err) {
			log("INVALID   ~ ", responseText, false)
			console.error(err)
			fadeIn();
			notify("{% t settings.messages.E2 %} (E2)");
			return
		}
		ssid.value = res.ssid;
		password.value = res.password;
		ent_username.value = res.entUsername;
		ent_identity.value = res.entIdentity;
		apSsid.value = res.apSsid;
		apPassword.value = res.apPassword;
		staticIP.value = res.staticIP;
		subnetMask.value = res.subnetMask;
		gateway.value = res.gateWay;
		macAp.value = res.macAp;
		apIP.value = res.apIP;
		dnsIP.value = res.dnsIP;
		randMacAp.checked = res.randMacAp;
		CustomDns.checked = res.CustomDns;
		ledEnable.checked = res.ledEnable;
		darkMode.checked = res.darkMode;
		webServer.checked = res.webServer;
		adminUsername.value = res.authUsername;
		adminPassword.value = res.authPassword
		maxLoginAttempts.value = res.maxLoginAttempts
		blockingTimeMin.value = res.blockingTimeMin
		switchMAC();
		dns_Ip();
		// nextDNS();
		fadeIn();
	}, function () {
		notify("{% t settings.messages.E4 %}  (E4)");
		fadeIn();
	});
}

function saveSettings() {
	showLoading();
	var url = '{% if jekyll.environment == "development" %}settingsSave.json{% else %}data/settingsSave.json{% endif %}';
	url += "?ssid=" + ssid.value;
	url += "&password=" + password.value;
	url += "&ent_username=" + ent_username.value;
	url += "&ent_password=" + ent_identity.value;
	url += "&apSsid=" + apSsid.value;
	url += "&apPassword=" + apPassword.value;
	url += "&staticIP=" + staticIP.value;
	url += "&subnetMask=" + subnetMask.value;
	url += "&gateway=" + gateway.value;
	url += "&apIP=" + apIP.value;
	url += "&macAp=" + macAp.value;
	url += "&dnsIP=" + dnsIp.value;
	url += "&adminUsername=" + adminUsername.value;
	url += "&adminPassword=" + adminPassword.value;
	url += "&maxLoginAttempts=" + maxLoginAttempts.value;
	url += "&blockingTimeMin=" + blockingTimeMin.value;
	url += "&ledEnable=" + ledEnable.checked;
	url += "&randMacAp=" + randMacAp.checked;
	url += "&CustomDns=" + CustomDns.checked;
	url += "&darkMode=" + darkMode.checked;
	url += "&webServer=" + webServer.checked;


	getResponse(url, function (responseText) {
		if (responseText == "true") {
			getData();
			indicate(true);
			var uniqueKey = new Date();
			document.getElementById('darkStyle').setAttribute('href', 'dark.css?' + uniqueKey.getTime());
			defaultMetaColor();
			inputChanges = false;
			checkboxChanges = false;
		} else {
			indicate(false);
			notify("{% t settings.messages.E0 %} (E0)", 2000);
		}
	}, function () {
		getResponse('{% if jekyll.environment == "development" %}settings.json{% else %}data/settings.json{% endif %}', function (responseText) {
			getData();
			indicate(true);
			var uniqueKey = new Date();
			document.getElementById('darkStyle').setAttribute('href', 'dark.css?' + uniqueKey.getTime());
			defaultMetaColor();
			inputChanges = false;
			checkboxChanges = false;
		}, function () {
			indicate(false);
			notify("{% t settings.messages.E3 %} (E3)");
		});
	},null,"POST");
}

function resetSettings() {
	if (confirm("{% t settings.strings.A %}") == true) {
		showLoading();
		getResponse('{% if jekyll.environment == "development" %}settingsReset.json{% else %}data/settingsReset.json{% endif %}', function (responseText) {
			if (responseText == "true") {
				getData();
				indicate(true);
				restart(true);
			} else {
				notify("{% t settings.messages.E1 %} (E1)", 2500);
				indicate(false);
			}
		}, function () {
			notify("{% t settings.messages.E4 %} (E4)");
			indicate(false);
		});
	}
	inputChanges = false;
	checkboxChanges = false;
}


function switchMAC() {
	if (randMacAp.checked)
		macContainer.classList.add("disabled");
	else
		macContainer.classList.remove("disabled");
}
function dns_Ip() {
	if (CustomDns.checked) {
		dnsIpContainer.classList.remove("disabled");
	} else {
		dnsIpContainer.classList.add("disabled");
	}
}

getData();

/* Detect form changes and display popup if not saved */
var form = document.getElementById("settings");
form.addEventListener("input", function () {
	inputChanges = true;
});
form.addEventListener("change", function () {
	checkboxChanges = true;
}, false);

window.addEventListener("beforeunload", function (e) {
	if (inputChanges || checkboxChanges) {
		var confirmationMessage = '{% t settings.strings.B %}';
		(e || window.event).returnValue = confirmationMessage;
		return confirmationMessage;
	}
});

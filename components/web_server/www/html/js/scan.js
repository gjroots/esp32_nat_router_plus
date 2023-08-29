---
---
var table = document.getElementsByTagName('table')[0],
	networkInfo = getE('networksFound'),
	scanInfo = getE('spinner-container'),
	apMAC = getE('apMAC'),
	selectAll = getE('selectAll'),
	ssid = getE("ssid"),
	password = getE("password"),
	// autoScan = true,
	tableHeaderHTML = '<tr><th width="17%">{% t scan.card-1.table.signal %}</th><th width="22%">{% t global.ssid %}</th><th width="15%">{% t scan.card-1.table.security %}</th><th width="8%">{% t scan.card-1.table.channel %}</th></tr>',
	selectedApElement = -1,
	previousCall = new Date().getTime(),
	url = window.location.href,
	wifiIndicator, securityState,
	scanning = false, res;

function toggleScan(onoff) {
	if (onoff) {
		showLoading("hide");
		scanning = false;
	} else {
		showLoading();
		scanning = true;
	}
}

function compare(a, b) {
	if (a.r > b.r) return -1;
	if (a.r < b.r) return 1;
	return 0;
}


//         WIFI_AUTH_OPEN = 0,        	  /**< authenticate mode : open */
//         WIFI_AUTH_WEP = 1,             /**< authenticate mode : WEP */
//         WIFI_AUTH_WPA_PSK = 2,         /**< authenticate mode : WPA_PSK */
//         WIFI_AUTH_WPA2_PSK = 3,        /**< authenticate mode : WPA2_PSK */
//         WIFI_AUTH_WPA_WPA2_PSK = 4,    /**< authenticate mode : WPA_WPA2_PSK */
//         WIFI_AUTH_WPA2_ENTERPRISE = 5, /**< authenticate mode : WPA2_ENTERPRISE */
//         WIFI_AUTH_WPA3_PSK = 6,        /**< authenticate mode : WPA3_PSK */
//         WIFI_AUTH_WPA2_WPA3_PSK = 7,   /**< authenticate mode : WPA2_WPA3_PSK */
//         WIFI_AUTH_WAPI_PSK,

function getStatus(enc, hid) {
	var buff = "";
	if (enc == 0) buff = "Open";
	else if (enc == 1) buff = "Wep&nbsp;&#128274;";
	else if (enc == 2) buff = "Wpa&nbsp;&#128274;";
	else if (enc == 3) buff = "Wpa2 &nbsp;&#128274;";
	else if (enc == 4) buff = "Wpa&nbsp;/&nbsp;Wpa2&nbsp;&#128274;";
	else if (enc == 5) buff = "WPA2&nbsp;EP;&#128274;";
	else if (enc == 6) buff = "Wpa3_PSK&nbsp;&#128274;";
	else if (enc == 7) buff = "Wpa2&nbsp;/&nbsp;Wpa3&nbsp;&#128274;";
	if (hid == 1) buff += "&#128123;";
	return buff
}
function scan() {
	if (scanning) return;
	toggleScan(false);
	getResponse('{% if jekyll.environment == "development" %}APScanResults.json{% else %}data/APScanResults.json{% endif %}', function (responseText, status) {
		try {
			if (responseText == "false") return setTimeout(function () { scanning = false;scan(); }, 1000);
			res = JSON.parse(responseText);
			log("RESPONSE  ~ ", res, true)
			notify();
			toggleScan(true);
		} catch (err) {
			log("INVALID   ~ ", responseText, false)
			console.error(err)
			notify('{% t scan.messages.E0 %} (E0)');
			scanning = false;
			return
		}
		res.aps = res.aps.sort(compare);
		networkInfo.innerHTML = res.aps.length;
		// if (res.aps.length == 0) scan()
		apMAC.innerHTML = "";
		if (res.multiAPs == 1) tableHeaderHTML = '<tr onclick><th width="17%">{% t scan.card-1.table.signal %}</th><th width="22%">{% t global.ssid %}</th><th width="15%">{% t scan.card-1.table.security %}</th><th width="8%">{% t scan.card-1.table.channel %}</th></tr>';
		var tr = '';
		if (res.aps.length > 0) tr += tableHeaderHTML;

		for (var i = 0; i < res.aps.length; i++) {

			// if (res.aps[i].se == 1) tr += '<tr class="selected" onclick="select(' + res.aps[i].i + ')">';
			tr += '<tr id = "tr' + i + '" class = "darken-on-hover" onclick="select(' + i + ')">';

			if (getStatus(res.aps[i].e) === "Open") { securityState = '' } else { securityState = 'L' }
			if (-89 > res.aps[i].r) {
				wifiIndicator = 's0' + securityState
			} else if (-88 > res.aps[i].r) {
				wifiIndicator = 's1' + securityState
			} else if (-77 > res.aps[i].r) {
				wifiIndicator = 's2' + securityState
			} else if (-66 > res.aps[i].r) {
				wifiIndicator = 's3' + securityState
			} else {
				wifiIndicator = 's4' + securityState
			}

			var signalPercent = Math.round((1 - ((res.aps[i].r + 30) / -70)) * 100);
			if (signalPercent > 100) signalPercent = 100;
			if (i == 0) { var tdID = ' id="resizeEventTD"' } else { var tdID = '' }
			tr += '<td class="WiFi"' + tdID + '><div>' + eval(wifiIndicator) + '</div><div><span style="background:linear-gradient(135deg, ' + getColor(signalPercent) + ' ' + signalPercent + '%,rgba(0,0,0,0.15) ' + signalPercent + '%)"></span><span style="color:' + getColor(signalPercent, true) + '">' + signalPercent + '</span></div></td>';
			tr += '<td>' + escapeHTML(res.aps[i].ss) + '</td>';
			tr += '<td>' + getStatus(res.aps[i].e, res.aps[i].h) + '</td>';
			tr += '<td>' + res.aps[i].c + '</td>';
			tr += '</tr>';
		}
		table.innerHTML = tr;
		checkSize()
		fadeIn();
	}, function () {
		toggleScan(true);
		fadeIn();
		notify("{% t scan.messages.E1 %} (E1)")
	});
}

function getColor(value, lighten) {
	var lightness = 50;
	var saturation = 75;
	if (lighten == true) lightness = 90
	if (value > 120) value = 100
	if (value > 90) saturation = 60;
	value = 100 - value;
	var hue = ((1 - (value / 87)) * 100).toString(10);
	return ["hsl(", hue, "," + saturation + "%," + lightness + "%)"].join("");
}


function select(num) {
	el = getE("tr" + num);
	getE("ssid").innerText = res.aps[num].ss;
	if (selectedApElement != -1) {
		selectedApElement.classList.remove("selected")
	}
	selectedApElement = el;
	el.classList.add("selected");
	getE("password").focus();

}


function connect() {
	if (selectedApElement == -1) return alert("{% t scan.strings.B %}");
	showLoading();
	var url = '{% if jekyll.environment == "development" %}settingsSave.json{% else %}data/settingsSave.json{% endif %}';
	url += "?ssid=" + encodeURI(ssid.innerText);
	url += "&password=" + encodeURI(password.value);
	getResponse(url, function (responseText) {
		if (responseText == "true") {
			indicate(true);
			restart(true);
			notify("{% t scan.messages.E2 %} (E2)", 2000);
		} else {
			indicate(false);
			notify("{% t scan.messages.E3 %} (E3)");
		}
	}, function () {
		ndicate(false);
		notify("{% t scan.messages.E4 %} (E4)");
	}, null, "POST");
}


/* Add event listener for WiFi signal icons */
window.onresize = function (event) { checkSize() }
function checkSize() {
	try {
		var w = document.getElementById('resizeEventTD');
		if (w.clientWidth <= 99) {
			document.getElementById('apscan').className = 'pointUp'
		} else {
			document.getElementById('apscan').className = ''
		}
	} catch (e) { }
}
scan();
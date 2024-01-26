---
---
var versionCell = getE("version"),
	ipGateway = getE("ipGatewayAddress"),
	ssid,
	ipAddress,
	dns,
	connection = getE("connection"),
	connectedUserCount = getE('clientsFound'),
	connectedUsers = getE('users'),
	filterList = getE("filterList"),
	filterListCount = getE("filterListCount"),
	ap_rss = 0,
	wifiAuthFail,
	tableHeaderHTML,
	otaProgress = false,
	filterListType,
	clients = [],
	filterListData = [];

function getData() {
	if (otaProgress) return;
	getResponse('{% if jekyll.environment == "development" %}sysinfo.json{% else %}data/sysinfo.json{% endif %}',
		function (responseText) {
			notify();
			try {
				var res = JSON.parse(responseText);
				log("RESPONSE  ~ ", res, true)
			} catch (err) {
				log("INVALID   ~ ", responseText, false)
				console.error(err)
				return
			}
			// console.log(res.clients.length)
			ssid = res.ssid;
			ipAddress = res.ipAddress;
			dns = res.dns;
			ap_rss = res.rss;
			wifiAuthFail = res.wifiAuthFail;
			clients = res.clients;
			versionCell.innerHTML = version;
			filterListData = res.filterList;
			filterListType = res.filterListType;
			fadeIn();
			users();
			ap_connection();
			filter_list(); 

		}, function () {
			notify('{% t info.messages.E0 %} (E0)');
			fadeIn();
		});
}

function bytesToSize(bytes) {
	var sizes = [' bytes', 'KB', 'MB', 'GB', 'TB'];
	if (bytes == 0) return '0 Byte';
	var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
	return Math.round(bytes / Math.pow(1024, i), 2) + '' + sizes[i];
};

getData();
infoInterval = setInterval(getData, 3000);



function upload_file() {
	var data = getE("file_select").files[0];

	request = new XMLHttpRequest();

	request.open("POST", "/ota", true);

	request.setRequestHeader('X-Requested-With', 'XMLHttpRequest');

	request.upload.addEventListener("progress", function (event) {
		otaProgress = true;

		if (event.lengthComputable) {
			var progress = Math.floor((event.loaded / event.total) * 100);
			notify("{{site.translations[site.lang].info.messages.message-1 | replace_first: '$$', '" + progress + " %'}}");
			showLoading();
		}

	});

	request.onreadystatechange = function () {

		if (request.readyState === XMLHttpRequest.DONE) {

			var status = request.status;

			if (status >= 200 && status < 400) {

				notify("{% t info.messages.message-2 %}", 2000);
				fadeIn();
				indicate(true);
				otaProgress = false;
				autoReload();

			} else {

				notify("{% t info.messages.E1 %} (E1)", 2000)
				fadeIn();
				indicate(false);
				data = "";
				otaProgress = false;
				autoReload();

			}

		}

	};

	request.send(data);
	return false;
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

function ap_connection() {
	var state = (ap_rss != 0);
	var svgImg = state ? soC : soD;
	var signalPercent = ap_rss;
	if (ap_rss != 0) {
		signalPercent = Math.round((1 - ((ap_rss + 30) / -70)) * 100);
		if (signalPercent > 100) signalPercent = 100;
	}

	var tr = '<tr><td>Wifi status</td>';
	tr += '<td><div>' + svgImg + '</div><div><span style="background:linear-gradient(135deg, ' + getColor(signalPercent) + ' ' + signalPercent + '%,rgba(0,0,0,0.15) ' + signalPercent + '%)"></span><span style="color:' + getColor(signalPercent, true) + '">' + signalPercent + '</span></div></td>';
	tr += '<td class=' + (state ? "green" : "red") + '><b>' + (state ? ssid : (wifiAuthFail ? "wifi Auth Fail" : 'Disconnected')) + '<b></td><tr>';
	connection.innerHTML = tr;

	tr = '<tr><td>{% t info.card-1.table.C %}</td><td><b>' + ipAddress + '</b></td><td>' + dns + '</td></tr>';
	ap_rss != 0 ? ipGateway.innerHTML = tr : "";
}
function users() {
	console.log("clients: ", clients.length)
	connectedUserCount.innerHTML =  clients.length;
	var tr = '';
	if (clients.length > 0) {
		tableHeaderHTML = '<tr><th>{% t info.card-2.table.A %}</th><th>{% t info.card-2.table.B %}</th><th>{% t info.card-2.table.C %}</th></tr>';
		tr += tableHeaderHTML;

		for (var i = 0; i < clients.length; i++) {
			var id = i + 1;
			tr += '<tr><td>' + clients[i].ipAddress + ' </td><td>' + clients[i].macAddress + '</td><td><button onclick="add_mac_to_filter_list('+i+');">{% t info.card-2.table.C %}</button></td></tr>';
			connectedUsers.innerHTML = tr;
		}
	}
}


function filter_list() {
	console.log("clients: ", filterListData.length)
	filterListCount.innerHTML =  filterListData.length;
	var tr = '';
	tableHeaderHTML = '<tr><th>{% t info.card-4.table.A %}</th><th>{% t info.card-4.table.B %}</th><th>{% t info.card-4.table.C %}</th></tr>';
		tr += tableHeaderHTML;
		filterList.innerHTML = tr;
	if (filterListData.length > 0) {

		for (var i = 0; i < filterListData.length; i++) {
			var id = i + 1;
			tr += '<tr><td>' + id + '</td><td>' + filterListData[i] + ' </td><td><button onclick="remove_mac_to_filter_list('+i+');">{% t info.card-4.table.C %}</button></td></tr>';
			filterList.innerHTML = tr;
		}
	}
	var IsAllowList = (filterListType === "Allow");
	tr += '<tr><td></td>'
	tr += '<td><label for="Allow" class="radioContainer">Alow&nbsp <input type="radio" name="filter_type" onchange="filter_list_type()" id="Allow"'+ (IsAllowList? "checked":"") + '><label class="radio" for="Allow"></label></label></td>';
	tr += '<td><label for="Deny" class="radioContainer">Deny&nbsp <input type="radio" name="filter_type" onchange="filter_list_type()"  id="Deny"'+ (IsAllowList? "":"checked") +'><label class="radio" for="Deny"></label></label></td>';
	tr += '</tr>'
	filterList.innerHTML = tr;
}


function saveSettings(data) {
	var url = '{% if jekyll.environment == "development" %}settingsSave.json{% else %}data/settingsSave.json{% endif %}';
	url += data;	
	showLoading();
		getResponse(url, function (responseText) {
			if (responseText == "true") {
				indicate(true);
				notify("{% t info.messages.message-3 %} (M3)", 2000);
			} else {
				indicate(false);
				notify("{% t info.messages.E2 %} (E2)");
			}
		}, function () {
			indicate(false);
			notify("{% t info.messages.E3 %} (E3)");
		}, null, "POST");
	}
	

function add_mac_to_filter_list(num){
	var data = "?add_mac_address=" + encodeURI(clients[num].macAddress);
	saveSettings(data); 
}

function remove_mac_to_filter_list(num){
	var data = "?remove_mac_address=" + encodeURI(filterListData[num]);
	saveSettings(data); 
}

function filter_list_type(){
	var data = "?filter_list_type="+ encodeURI(getE("Allow").checked ? "Allow":"Deny")
	saveSettings(data);
}

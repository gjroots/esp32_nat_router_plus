---
---
var version="1.2.0",
	sL = getE('spinner-container'),
	notification = getE("notification"),
	themeColor = getComputedStyle(document.body),
	saveStatus = getE('spinner-container'),
	mainWrap = document.getElementsByClassName("main-wrap")[0];

console.log('%c╔═╗┌─┐┌─┐   ╔╗╔┌─┐┌┬┐ ╦═╗┌─┐┬ ┬┌┬┐┌─┐┬─┐ _|_ \n║╣ └─┐├─┘32─║║║├─┤ │  ╠╦╝│ ││ │ │ ├┤ ├┬┘  |  \n╚═╝└─┘┴     ╝╚╝┴ ┴ ┴  ╩╚═└─┘└─┘ ┴ └─┘┴└─\n%c                   Version %c' + version + '\n\n%cSource code available at\n%cwww.github.com/gjroots', 'color:#369BF2', 'color:#ff6c00', 'color:#ce5a2c', 'color:#359c64', 'color:#2dc572')

function notify(msg, closeAfter) {
	if (msg) {
		notification.innerHTML = msg;
		document.body.className = "show";
	} else {
		document.body.className = "";
	}
	if (closeAfter !== undefined) {
		setTimeout(function () {
			document.body.classList.remove("show")
		}, closeAfter);
	}
}

function indicate(indState) {
	if (indState == null) {
		saveStatus.classList.remove("show-loading");
		setTimeout(function () { saveStatus.className = "" }, 500)
	} else if (indState == true) {
		saveStatus.classList.add("show-loading");
		saveStatus.classList.add("success-save");
		setTimeout(function () { indicate() }, 2500);
	} else if (indState == false) {
		saveStatus.classList.add("show-loading");
		saveStatus.classList.add("failed-save");
		setTimeout(function () { indicate() }, 2500);
	}
}

function defaultMetaColor() {
	try {
		themeColor = getComputedStyle(document.body);
		themeColor = themeColor.getPropertyValue('--theme-color');
	} catch (err) {
		themeColor = '#106CC8';
	}
	var mC = document.querySelector("meta[name=theme-color]");
	mC.setAttribute("content", themeColor);
}

function log(one, two, three) {
	var color = "#369BF2", success
	if (typeof three !== 'undefined')
		success = three
	else
		success = two
	if (success != "default" && typeof success !== 'undefined') {
		if (success == true)
			color = "#3ec563"
		else
			color = "#de363c"
	}
	if (typeof three !== 'undefined')
		console.log('%c[ESP32-NAT Router +] %c' + one, 'color:' + color, 'color:#777', two)
	else
		console.log('%c[ESP32-NAT Router +] ', 'color:' + color, one)
}


function checkConnection() {
	setTimeout(function () { getResponse('{% if jekyll.environment == "development" %}ClientScanTime.json{% else %}data/ClientScanTime.json{% endif %}', function (responseText) { window.location.reload(); notify("{% t errors.S2 %}",2000) }, function () { notify("{% t errors.E11 %} (E11)"); checkConnection() }, 2000) }, 1300);
}

function autoReload() {
	setTimeout(function () { getResponse('{% if jekyll.environment == "development" %}ClientScanTime.json{% else %}data/ClientScanTime.json{% endif %}', function (responseText) { window.location.reload();  notify("{% t errors.S3 %}",2000) }, function () { autoReload() }) }, 3000);
}

function restart(noIndication) {
	if (noIndication == true) {
		showLoading();
		getResponse('{% if jekyll.environment == "development" %}restart.json{% else %}data/restart.json{% endif %}', function (responseText) {
			if (responseText == "true") {
				notify("{% t errors.E12 %} (E12)", 2000);
				autoReload();
				showLoading("hide");
			}
		}, function () {
			notify("{% t errors.E13 %} (E13)");
			showLoading("hide");
		});
	} else {
		if (confirm("{% t errors.S1 %}") == true) {
			vibrate();
			sL.className = "";
			showLoading();
			autoReload();
			getResponse('{% if jekyll.environment == "development" %}restart.json{% else %}data/restart.json{% endif %}', function (responseText) {
				if (responseText == "true") {
					notify("{% t errors.E12 %} (E12)", 2000);
					showLoading();
				}
			}, function () {
				notify("{% t errors.E13 %} (E13)");
				showLoading("hide");
			});
		}
	}
}

function getE(name) {
	return document.getElementById(name);
}

function escapeHTML(str) {
	return str
		.replace(/&/g, '&amp;')
		.replace(/</g, '&lt;')
		.replace(/>/g, '&gt;')
		.replace(/\"/g, '&quot;')
		.replace(/\'/g, '&#39;')
		.replace(/\//g, '&#x2F;')
		.replace(/\+/g, '%2b')
}

function showLoading(state) {
	if (state == "hide") {
		sL.className = "";
	} else {
		sL.classList.add("show-loading");
	}
}

function getResponse(adr, callback, timeoutCallback, timeout, method) {
	log('FETCH     ~ ', adr, "default")
	if (timeoutCallback == null) {
		timeoutCallback = function () {
			getResponse('{% if jekyll.environment == "development" %}ClientScanTime.json{% else %}data/ClientScanTime.json{% endif %}', function () {
				notify("{% t errors.E14 %} (E14)");
			}, function () {
				notify("{% t errors.E15 %} (E15)");
				autoReload();
			})
		};
	}
	if (timeout == null) timeout = 8000;
	if (method == null) method = "GET";
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function () {
		if (xmlhttp.readyState == 4) {
			if (xmlhttp.status == 200) {
				if (typeof callback !== 'undefined') callback(xmlhttp.responseText);
			} else timeoutCallback();
		}
	};
	xmlhttp.open(method, adr, true);
	xmlhttp.send();
	xmlhttp.timeout = timeout;
	xmlhttp.ontimeout = timeoutCallback;
}

function scrollIt(element) {
	if (window.innerWidth <= 520) {
		var displayType = window
	} else {
		var displayType = document.getElementsByClassName("main-wrap")[0]
	}
	try {
		displayType.scrollTo({
			'behavior': 'smooth',
			'left': 0,
			'top': element.offsetTop - 25
		});
	} catch (err) {
		element.scrollIntoView()
		window.scrollBy(0, -75);
	}
}


function vibrate() {
	navigator.vibrate = navigator.vibrate || navigator.webkitVibrate || navigator.mozVibrate || navigator.msVibrate;
	if (navigator.vibrate) {
		navigator.vibrate([40, 60, 70]);
	}
}

function fadeIn() {
	if (!mainWrap.className.match('fadeIn')) {
		setTimeout(function () { document.getElementsByClassName('main-wrap')[0].classList.add('fadeIn') }, 10)
		setTimeout(function () { document.getElementsByClassName('main-wrap')[0].classList.add('animation') }, 1000)
	}
}

/* Set meta color */
defaultMetaColor();

/* Dynamically add favicon (low-res) */
var link = document.createElement('link');
link.type = 'image/x-icon';
link.rel = 'shortcut icon';
link.href = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAASFBMVEVHcExCoP47kPA7kPA7kPI7kPA7kPA6j/A6kPA7kPA7kPFHrf8AAAA/m/0MHzMHERwZP2YjWJIsbLI3h90RKUUpZaQcRnUUMlKQkJ9vAAAACnRSTlMA8CEp6RnzudZa1JTcaQAAAKBJREFUGNNVj1sSwiAMRanSQm8ePAv736nB6jieP0KSc+OccYZ4xHC6D3vATXjcbw+hheDYVyGC5Bo5FxDimgfVxKyJUyXYnkCFuXftnfmi4Nx2ca5Z88y1c9usUC7YSFZWmZmc80IplZl6YaXmbQdQMVl5tCESlsU0rFPZtMtiOQAZqbSGdw63H9YzTAkcd/aH9UgT+39+zzuD3/zv2n9eKc4JINVshQgAAAAASUVORK5CYII';
document.getElementsByTagName('head')[0].appendChild(link);

/* Dynamically add footer */
document.getElementsByTagName("footer")[0].innerHTML = "<div class=footer><ul><li><b>ESP32-NAT Router + </b> &copy; 2023</a><li><a href=https://github.com/gjroots/esp32_nat_router_plus target=blank_>GitHub</a> </ul></div><a href=https://github.com/gjroots/ target=blank_ class=sub-section-attribution>{% t global.footer %}{% if site.translations[site.lang].translation.name %}, {{site.translations[site.lang].global.translated-by | replace_first: '$$', site.translations[site.lang].translation.name }}.{% endif %}</a>";

/* Dynamically add spinner */
getE("spinner-container").innerHTML = "<svg class=spinner viewBox='0 0 66 66'><circle class=path cx=33 cy=33 fill=none r=30 stroke-linecap=round stroke-width=6></circle></svg> <svg class=success-svg viewBox='0 0 1000 1000'><path d=M908.3,132.5L336.7,704.2l-245-245L10,540.8l326.7,326.7l81.7-81.7L990,214.2L908.3,132.5z /></svg> <svg class=failed-svg viewBox='0 0 19 19'><line stroke-width=2 x1=1 x2=18 y1=1 y2=18></line><line stroke-width=2 x1=18 x2=1 y1=1 y2=18></line></svg>"

/* Dynamically add SVG version of Wi-PWN logo */
// var svgLogo = '<svg height=100% viewBox="0 0 48 48"width=100%><path d="M47.28 14c-.9-.68-9.85-8-23.28-8-3.01 0-5.78.38-8.3.96l20.66 20.64 10.92-13.6zm-40.73-11.11l-2.55 2.55 4.11 4.11c-4.28 1.97-6.92 4.1-7.39 4.46l23.26 28.98.02.01.02-.02 7.8-9.72 6.63 6.63 2.55-2.55-34.45-34.45z"></path></svg>'
var svgLogo = '<svg height=100%  viewBox="0 0 48 48" xmlns="http://www.w3.org/2000/svg" width=100%><path d="M24.02 42.98L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02z"></path></svg>'
getE("logo-img").innerHTML = svgLogo;


/* Dynamically add reboot button */
document.body.insertAdjacentHTML('afterbegin', '<div class="reboot-container"><div class=reboot-inner><svg onclick="restart()" class=reboot viewBox="0 0 24 24"><path d="M17.65,6.35C16.2,4.9 14.21,4 12,4A8,8 0 0,0 4,12A8,8 0 0,0 12,20C15.73,20 18.84,17.45 19.73,14H17.65C16.83,16.33 14.61,18 12,18A6,6 0 0,1 6,12A6,6 0 0,1 12,6C13.66,6 15.14,6.69 16.22,7.78L13,11H20V4L17.65,6.35Z"/></svg></div><span class="tooltip">{% t global.reboot %}</span></div>');

/* Compressed Material design WiFi icons generator
 * AUTHOR: SAM DENTY         github.com/samdenty99
 *
 * WiFi icons are stored in the variables:
 * s0, s1, s2, s3, s4   |   s0L, s1L, s2L, s3L, s4L
 *
 */
/* Universal Variables */
var sS = '<svg viewBox="0 0 48 48"xmlns=http://www.w3.org/2000/svg>';
var sC = '<svg viewBox="0 0 24 24"xmlns=http://www.w3.org/2000/svg>'
var sN = '<path d="M24.02 42.98L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02z"fill-opacity=.3 />'
var sE = '</svg>'
/* Combine strings to form WiFi icons without lock icon */
var s0 = sS + sN + sE;
var s1 = sS + sN + '<path d="M13.34 29.72l10.65 13.27.01.01.01-.01 10.65-13.27C34.13 29.31 30.06 26 24 26s-10.13 3.31-10.66 3.72z"/>' + sE;
var s2 = sS + sN + '<path d="M9.58 25.03l14.41 17.95.01.02.01-.02 14.41-17.95C37.7 24.47 32.2 20 24 20s-13.7 4.47-14.42 5.03z"/>' + sE;
var s3 = sS + sN + '<path d="M7.07 21.91l16.92 21.07.01.02.02-.02 16.92-21.07C40.08 21.25 33.62 16 24 16c-9.63 0-16.08 5.25-16.93 5.91z"/>' + sE;
var s4 = sS + '<path d="M24.02 42.98L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02z"/>' + sE;
/* Combine strings to form WiFi icons with lock icon */
var s0L = sS + '<path d="m41,19c0.7,0 1.4,0.1 2.1,0.2l4.2,-5.2c-0.9,-0.7 -9.8,-8 -23.3,-8s-22.4,7.3 -23.3,8l23.3,29l7,-8.7l0,-5.3c0,-5.5 4.5,-10 10,-10z"opacity=0.3 /><path d="m46,32l0,-3c0,-2.8 -2.2,-5 -5,-5s-5,2.2 -5,5l0,3c-1.1,0 -2,0.9 -2,2l0,8c0,1.1 0.9,2 2,2l10,0c1.1,0 2,-0.9 2,-2l0,-8c0,-1.1 -0.9,-2 -2,-2zm-2,0l-6,0l0,-3c0,-1.7 1.3,-3 3,-3s3,1.3 3,3l0,3z">' + sE;
var s1L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.8-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zm-30.7-2.3l9 11.2L24 43l7-8.8V29c0-.5 0-1 .1-1.4-1.8-.8-4.2-1.6-7.1-1.6-6.1 0-10.1 3.3-10.7 3.7z">' + sE;
var s2L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.9-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zM9.6 25L24 43l7-8.7V29c0-2.6 1-5 2.7-6.8C31.2 21 27.9 20 24 20c-8.2 0-13.7 4.5-14.4 5z">' + sE;
var s3L = sS + '<path d="M41 19c.7 0 1.4.1 2.1.2l4.2-5.2c-.9-.7-9.8-8-23.3-8S1.6 13.3.7 14L24 43l7-8.7V29c0-5.5 4.5-10 10-10z"enable-background=new opacity=.3 /><path d="M46 32v-3c0-2.8-2.2-5-5-5s-5 2.2-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.7 1.3-3 3-3s3 1.3 3 3v3zM7.1 21.9L24 43l7-8.7V29c0-4.3 2.7-8 6.5-9.4C34.6 18 29.9 16 24 16c-9.6 0-16.1 5.2-16.9 5.9z">' + sE;
var s4L = sS + '<path d="M41 19c.72 0 1.41.08 2.09.22L47.28 14c-.9-.68-9.85-8-23.28-8S1.62 13.32.72 14l23.26 28.98.02.02.02-.02 6.98-8.7V29c0-5.52 4.48-10 10-10zm5 13v-3c0-2.76-2.24-5-5-5s-5 2.24-5 5v3c-1.1 0-2 .9-2 2v8c0 1.1.9 2 2 2h10c1.1 0 2-.9 2-2v-8c0-1.1-.9-2-2-2zm-2 0h-6v-3c0-1.66 1.34-3 3-3s3 1.34 3 3v3z">' + sE;
var soC = sC + '<path d="M4.93,3.93C3.12,5.74 2,8.24 2,11C2,13.76 3.12,16.26 4.93,18.07L6.34,16.66C4.89,15.22 4,13.22 4,11C4,8.79 4.89,6.78 6.34,5.34L4.93,3.93M19.07,3.93L17.66,5.34C19.11,6.78 20,8.79 20,11C20,13.22 19.11,15.22 17.66,16.66L19.07,18.07C20.88,16.26 22,13.76 22,11C22,8.24 20.88,5.74 19.07,3.93M7.76,6.76C6.67,7.85 6,9.35 6,11C6,12.65 6.67,14.15 7.76,15.24L9.17,13.83C8.45,13.11 8,12.11 8,11C8,9.89 8.45,8.89 9.17,8.17L7.76,6.76M16.24,6.76L14.83,8.17C15.55,8.89 16,9.89 16,11C16,12.11 15.55,13.11 14.83,13.83L16.24,15.24C17.33,14.15 18,12.65 18,11C18,9.35 17.33,7.85 16.24,6.76M12,9A2,2 0 0,0 10,11A2,2 0 0,0 12,13A2,2 0 0,0 14,11A2,2 0 0,0 12,9M11,15V19H10A1,1 0 0,0 9,20H2V22H9A1,1 0 0,0 10,23H14A1,1 0 0,0 15,22H22V20H15A1,1 0 0,0 14,19H13V15H11Z" />' + sE;
var soD = sC + '<path d="M14.83,13.83C15.55,13.11 16,12.11 16,11C16,9.89 15.55,8.89 14.83,8.17L16.24,6.76C17.33,7.85 18,9.35 18,11C18,12.65 17.33,14.15 16.24,15.24L14.83,13.83M14,11A2,2 0 0,0 12,9C11.4,9 10.87,9.27 10.5,9.68L13.32,12.5C13.73,12.13 14,11.6 14,11M17.66,16.66L19.07,18.07C20.88,16.26 22,13.76 22,11C22,8.24 20.88,5.74 19.07,3.93L17.66,5.34C19.11,6.78 20,8.79 20,11C20,13.22 19.11,15.22 17.66,16.66M22,21.18V20H20.82L22,21.18M20.27,22L21,22.73L19.73,24L17.73,22H15A1,1 0 0,1 14,23H10A1,1 0 0,1 9,22H2V20H9A1,1 0 0,1 10,19H11V15.27L8.34,12.61C8.54,13.07 8.82,13.5 9.17,13.83L7.76,15.24C6.67,14.15 6,12.65 6,11C6,10.77 6,10.54 6.04,10.31L4.37,8.64C4.14,9.39 4,10.18 4,11C4,13.22 4.89,15.22 6.34,16.66L4.93,18.07C3.12,16.26 2,13.76 2,11C2,9.61 2.29,8.28 2.81,7.08L1,5.27L2.28,4L3.7,5.42L5.15,6.87L6.63,8.35V8.35L8.17,9.9L10.28,12L11,12.71L18.27,20H18.28L20.28,22H20.27M15.73,20L13,17.27V19H14A1,1 0 0,1 15,20H15.73Z"/>'
/* Waves Library */
!function (a, b) { "use strict"; "function" == typeof define && define.amd ? define([], function () { return b.apply(a) }) : "object" == typeof exports ? module.exports = b.call(a) : a.Waves = b.call(a) }("object" == typeof global ? global : this, function () { "use strict"; function a(a) { return null !== a && a === a.window } function b(b) { return a(b) ? b : 9 === b.nodeType && b.defaultView } function c(a) { var b = typeof a; return "function" === b || "object" === b && !!a } function d(a) { return c(a) && a.nodeType > 0 } function e(a) { var b = m.call(a); return "[object String]" === b ? l(a) : c(a) && /^\[object (Array|HTMLCollection|NodeList|Object)\]$/.test(b) && a.hasOwnProperty("length") ? a : d(a) ? [a] : [] } function f(a) { var c, d, e = { top: 0, left: 0 }, f = a && a.ownerDocument; return c = f.documentElement, "undefined" != typeof a.getBoundingClientRect && (e = a.getBoundingClientRect()), d = b(f), { top: e.top + d.pageYOffset - c.clientTop, left: e.left + d.pageXOffset - c.clientLeft } } function g(a) { var b = ""; for (var c in a) a.hasOwnProperty(c) && (b += c + ":" + a[c] + ";"); return b } function h(a, b, c) { if (c) { c.classList.remove("waves-rippling"); var d = c.getAttribute("data-x"), e = c.getAttribute("data-y"), f = c.getAttribute("data-scale"), h = c.getAttribute("data-translate"), i = Date.now() - Number(c.getAttribute("data-hold")), j = 350 - i; 0 > j && (j = 0), "mousemove" === a.type && (j = 150); var k = "mousemove" === a.type ? 2500 : o.duration; setTimeout(function () { var a = { top: e + "px", left: d + "px", opacity: "0", "-webkit-transition-duration": k + "ms", "-moz-transition-duration": k + "ms", "-o-transition-duration": k + "ms", "transition-duration": k + "ms", "-webkit-transform": f + " " + h, "-moz-transform": f + " " + h, "-ms-transform": f + " " + h, "-o-transform": f + " " + h, transform: f + " " + h }; c.setAttribute("style", g(a)), setTimeout(function () { try { b.removeChild(c) } catch (a) { return !1 } }, k) }, j) } } function i(a) { if (q.allowEvent(a) === !1) return null; for (var b = null, c = a.target || a.srcElement; c.parentElement;) { if (!(c instanceof SVGElement) && c.classList.contains("waves-effect")) { b = c; break } c = c.parentElement } return b } function j(a) { var b = i(a); if (null !== b) { if (b.disabled || b.getAttribute("disabled") || b.classList.contains("disabled")) return; if (q.registerEvent(a), "touchstart" === a.type && o.delay) { var c = !1, d = setTimeout(function () { d = null, o.show(a, b) }, o.delay), e = function (e) { d && (clearTimeout(d), d = null, o.show(a, b)), c || (c = !0, o.hide(e, b)) }, f = function (a) { d && (clearTimeout(d), d = null), e(a) }; b.addEventListener("touchmove", f, !1), b.addEventListener("touchend", e, !1), b.addEventListener("touchcancel", e, !1) } else o.show(a, b), n && (b.addEventListener("touchend", o.hide, !1), b.addEventListener("touchcancel", o.hide, !1)), b.addEventListener("mouseup", o.hide, !1), b.addEventListener("mouseleave", o.hide, !1) } } var k = k || {}, l = document.querySelectorAll.bind(document), m = Object.prototype.toString, n = "ontouchstart" in window, o = { duration: 750, delay: 200, show: function (a, b, c) { if (2 === a.button) return !1; b = b || this; var d = document.createElement("div"); d.className = "waves-ripple waves-rippling", b.appendChild(d); var e = f(b), h = 0, i = 0; "touches" in a && a.touches.length ? (h = a.touches[0].pageY - e.top, i = a.touches[0].pageX - e.left) : (h = a.pageY - e.top, i = a.pageX - e.left), i = i >= 0 ? i : 0, h = h >= 0 ? h : 0; var j = "scale(" + b.clientWidth / 100 * 3 + ")", k = "translate(0,0)"; c && (k = "translate(" + c.x + "px, " + c.y + "px)"), d.setAttribute("data-hold", Date.now()), d.setAttribute("data-x", i), d.setAttribute("data-y", h), d.setAttribute("data-scale", j), d.setAttribute("data-translate", k); var l = { top: h + "px", left: i + "px" }; d.classList.add("waves-notransition"), d.setAttribute("style", g(l)), d.classList.remove("waves-notransition"), l["-webkit-transform"] = j + " " + k, l["-moz-transform"] = j + " " + k, l["-ms-transform"] = j + " " + k, l["-o-transform"] = j + " " + k, l.transform = j + " " + k, l.opacity = "1"; var m = "mousemove" === a.type ? 2500 : o.duration; l["-webkit-transition-duration"] = m + "ms", l["-moz-transition-duration"] = m + "ms", l["-o-transition-duration"] = m + "ms", l["transition-duration"] = m + "ms", d.setAttribute("style", g(l)) }, hide: function (a, b) { b = b || this; for (var c = b.getElementsByClassName("waves-rippling"), d = 0, e = c.length; e > d; d++)h(a, b, c[d]) } }, p = { input: function (a) { var b = a.parentNode; if ("i" !== b.tagName.toLowerCase() || !b.classList.contains("waves-effect")) { var c = document.createElement("i"); c.className = a.className + " waves-input-wrapper", a.className = "waves-button-input", b.replaceChild(c, a), c.appendChild(a); var d = window.getComputedStyle(a, null), e = d.color, f = d.backgroundColor; c.setAttribute("style", "color:" + e + ";background:" + f), a.setAttribute("style", "background-color:rgba(0,0,0,0);") } }, img: function (a) { var b = a.parentNode; if ("i" !== b.tagName.toLowerCase() || !b.classList.contains("waves-effect")) { var c = document.createElement("i"); b.replaceChild(c, a), c.appendChild(a) } } }, q = { touches: 0, allowEvent: function (a) { var b = !0; return /^(mousedown|mousemove)$/.test(a.type) && q.touches && (b = !1), b }, registerEvent: function (a) { var b = a.type; "touchstart" === b ? q.touches += 1 : /^(touchend|touchcancel)$/.test(b) && setTimeout(function () { q.touches && (q.touches -= 1) }, 500) } }; return k.init = function (a) { var b = document.body; a = a || {}, "duration" in a && (o.duration = a.duration), "delay" in a && (o.delay = a.delay), n && (b.addEventListener("touchstart", j, !1), b.addEventListener("touchcancel", q.registerEvent, !1), b.addEventListener("touchend", q.registerEvent, !1)), b.addEventListener("mousedown", j, !1) }, k.attach = function (a, b) { a = e(a), "[object Array]" === m.call(b) && (b = b.join(" ")), b = b ? " " + b : ""; for (var c, d, f = 0, g = a.length; g > f; f++)c = a[f], d = c.tagName.toLowerCase(), -1 !== ["input", "img"].indexOf(d) && (p[d](c), c = c.parentElement), -1 === c.className.indexOf("waves-effect") && (c.className += " waves-effect" + b) }, k.ripple = function (a, b) { a = e(a); var c = a.length; if (b = b || {}, b.wait = b.wait || 0, b.position = b.position || null, c) for (var d, g, h, i = {}, j = 0, k = { type: "mousedown", button: 1 }, l = function (a, b) { return function () { o.hide(a, b) } }; c > j; j++)if (d = a[j], g = b.position || { x: d.clientWidth / 2, y: d.clientHeight / 2 }, h = f(d), i.x = h.left + g.x, i.y = h.top + g.y, k.pageX = i.x, k.pageY = i.y, o.show(k, d), b.wait >= 0 && null !== b.wait) { var m = { type: "mouseup", button: 1 }; setTimeout(l(m, d), b.wait) } }, k.calm = function (a) { a = e(a); for (var b = { type: "mouseup", button: 1 }, c = 0, d = a.length; d > c; c++)o.hide(b, a[c]) }, k.displayEffect = function (a) { k.init(a) }, k });

/* Attach waves to elements */
Waves.attach('#links a', ['waves-light-color']);
Waves.attach('.card-action .button', ['waves-color']);
Waves.attach('.secondary', ['waves-color']);
Waves.attach('button', ['waves-light']);
Waves.attach('.button', ['waves-light']);
Waves.attach('.reboot-inner');
Waves.init();

/* Push page function */
function push(_p) {
	// If the page is different
	if (window.location.search != "?" + _p) {
		var _params = "";
		if (_p) var _params = "?" + _p;
		// Remove params from URL and add new ones
		var newurl = window.location.href.split("?")[0] + _params;
		if (history.pushState) {
			// Go to new URL without reloading
			window.history.pushState({ path: newurl }, null, newurl);
		} else {
			// Browser doesn't support pushState
			window.location = newurl;
		}
	}
};

/* Add scroll event listeners */
var nav = getE('nav');
if (nav) {
	window.onscroll = function () {
		if (window.pageYOffset > 3) {
			nav.classList.add("n-shadow");
		} else {
			nav.classList.remove("n-shadow");
		}
	}
	mainWrap.onscroll = function () {
		if (mainWrap.scrollTop > 3) {
			nav.classList.add("n-shadow");
		} else {
			nav.classList.remove("n-shadow");
		}
	}
}



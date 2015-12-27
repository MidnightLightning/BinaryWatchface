// Get a value from the query string, or a default value if not set
var getQueryParam = function(variableName, defaultValue) {
	var query = location.search.substring(1);
	var vars = query.split('&');
	for (var i = 0; i < vars.length; i++) {
		var pair = vars[i].split('=');
		if (pair[0] === variableName) {
			return decodeURIComponent(pair[1]);
		}
	}
	return defaultValue || false;
};

// Pull the current values from the input fields
var getAndStoreConfigData = function() {
	var options = {
		showDate: $('#showDate')[0].checked,
		showBluetooth: $('#showBluetooth')[0].checked,
		theme: $('#theme').val()
	};
	
	localStorage.showDate = options.showDate;
	localStorage.showBluetooth = options.showBluetooth;
	localStorage.theme = options.theme;
	
	return options;
};

var loadOptions = function() {
	if (localStorage.showDate) {
		$('#showDate')[0].checked = localStorage.showDate === 'true';
	}
	if (localStorage.showBluetooth) {
		$('#showBluetooth')[0].checked = localStorage.showBluetooth === 'true';
	}
	if (localStorage.theme) {
		$('#theme')[0].value = localStorage.theme;
	}
};

(function() {
	loadOptions();
	$('#sendBtn').on('click', function() {
		var return_to = getQueryParam('return_to', 'pebblejs://close#');
		document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
	});
})();


Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('http://midnight.design/binary-watchface/config/');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  Pebble.sendAppMessage({
    'KEY_SHOW_DATE': configData.showDate,
    'KEY_SHOW_BLUETOOTH': configData.showBluetooth,
    'KEY_THEME': parseInt(configData.theme)
  }, function() {
    console.log('Configuration update successful');
  }, function() {
    console.log('Configuration update failed');
  });
});
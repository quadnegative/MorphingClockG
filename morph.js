var configLoading = false;
var configUpdating = false;
var perviousPalette;

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
      r: parseInt(result[1], 16),
      g: parseInt(result[2], 16),
      b: parseInt(result[3], 16)
    } : null;
}

function loadConfig() {
    configLoading = true;
    $('#modalLabel')[0].innerText = 'Loading';
    $('.modal-footer').hide();
    $('.modal').modal('show');
    $.getJSON('/config', function(configJSON) {
        if (!jQuery.isEmptyObject({configJSON})) {
            $("#ConfigJSON")[0].innerText=JSON.stringify(configJSON, null, 4);
            if (configJSON.TimeZone) {
                if (configJSON.TimeZone.includes('ST')) {
                    $('#'+configJSON.TimeZone)[0].checked = true;
                    $('#CustomTZOffset').hide();
                }
                else {
                    $('#CustomTZ')[0].checked = true;
                    $('#CustomTZOffset')[0].value=configJSON.TimeZone;
                }
            }
            if (configJSON.DST) $('#DST')[0].checked = true;
            if (configJSON.Military) $('#Military')[0].checked = true;
            if (configJSON.Metric) $('#Celsius')[0].checked = true;
            else if (!configJSON.Metric) $('#Fahrenheit')[0].checked = true;
            if (configJSON.WeatherAnimation) $('#WeatherAnimation')[0].checked = true;
            if (configJSON.ColorPalette) {
                $('*[data-palette="'+configJSON.ColorPalette+'"]')[0].checked = true;
                perviousPalette =  configJSON.ColorPalette;
                if ($('input[name="ColorPalette"]:checked:enabled')[0].id.includes('Custom')) {
                    $('#ColorInput').show();
                }
                else {
                    $('#ColorInput').hide();
                }
            }
            if (configJSON.Brightness) $('*[data-brightness="'+configJSON.Brightness+'"]')[0].checked = true;
            if (configJSON.apiKey) $("#apiKey")[0].value=configJSON.apiKey;
            if (configJSON.GeoLocation) $("#GeoLocation")[0].value=configJSON.GeoLocation;
            if (configJSON.SSID) $("#SSID")[0].value=configJSON.SSID;
            if (configJSON.Password) $("#Password")[0].value=configJSON.Password;
            if (configJSON.NTPServer) $("#NTPServer")[0].value=configJSON.NTPServer;
            if (configJSON.Hostname) $("#Hostname")[0].value=configJSON.Hostname;
        }
    })
    .done(function() {
        $('.modal').modal('hide');
    })
    .fail(function() {
        $('.modal-body')[0].innerHTML = '<p>Load Failed!</p>';
    });
    configLoading = false;
}

function updateConfigJSON() {
    if (!configLoading && !configUpdating) {
        //configUpdating = true;
        //Save config
        configJSON = {};
        if ($('#ConfigJSON')[0].innerText) configJSON = JSON.parse($('#ConfigJSON')[0].innerText);
        if ($('input[name="TimeZone"]:checked:enabled')[0].id == "CustomTZ") {
            $('#CustomTZOffset').show();
            configJSON["TimeZone"] = $('input[id="CustomTZOffset"]')[0].value;
        }
        else {
            $('#CustomTZOffset').hide();
            configJSON["TimeZone"] = $('input[name="TimeZone"]:checked:enabled')[0].id;
        }
        configJSON["DST"] = $('input[id="DST"]')[0].checked;
        configJSON["Military"] = $('input[id="Military"]')[0].checked;
        if ($('input[name="TemperatureUnit"]:checked:enabled')[0] && $('input[name="TemperatureUnit"]:checked:enabled')[0].dataset.value == "false") configJSON["Metric"] = false;
        else configJSON["Metric"] = true;
        configJSON["WeatherAnimation"] = $('input[id="WeatherAnimation"]')[0].checked;
        //probably wrong order
        if ($('input[name="ColorPalette"]:checked:enabled')[0]) configJSON["ColorPalette"] = $('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette
        if ($('input[name="ColorPalette"]:checked:enabled')[0] && $('input[name="ColorPalette"]:checked:enabled')[0].id.includes('Custom')) {
            $('#ColorInput').show();
            configJSON["ColorPalette"+$('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette] = {
                "time":hexToRgb($('input[id="TimeColorInput"]')[0].value),
                "wind":hexToRgb($('input[id="WindColorInput"]')[0].value),
                "date":hexToRgb($('input[id="DateColorInput"]')[0].value),
                "weather":hexToRgb($('input[id="WeatherColorInput"]')[0].value)
            }
        }
        else {
            $('#ColorInput').hide();
        }
        if ($('input[name="Brightness"]:checked:enabled')[0]) configJSON["Brightness"] = $('input[name="Brightness"]:checked:enabled')[0].dataset.brightness
        configJSON["apiKey"] = $('input[id="apiKey"]')[0].value;
        configJSON["GeoLocation"] = $('input[id="GeoLocation"]')[0].value;
        configJSON["SSID"] = $('input[id="SSID"]')[0].value;
        configJSON["Password"] = $('input[id="Password"]')[0].value;
        configJSON["NTPServer"] = $('input[id="NTPServer"]')[0].value;
        configJSON["Hostname"] = ($('input[id="Hostname"]')[0].value).toLowerCase();
        configJSON["DateFormat"] = "M.D.Y";
        $('#ConfigJSON')[0].innerText=JSON.stringify(configJSON, null, 4);
        //Load Updates
        //probably wrong order
        // if ($('input[name="ColorPalette"]:checked:enabled')[0] && $('input[name="ColorPalette"]:checked:enabled')[0].id.includes('Custom')) {
        //     if (configJSON["ColorPalette"+$('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette]) {
        //         console.log("custom palette " + $('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette + " found!");
        //         //Load that custom palette
        //         //$('input[id="TimeColorInput"]')[0].value = rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette]["time"]["r"], configJSON["ColorPalette"+configJSON.ColorPalette]["time"]["g"], configJSON["ColorPalette"+configJSON.ColorPalette]["time"]["b"])
        //         //$('input[id="WindColorInput"]')[0].value = rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette]["wind"]["r"], configJSON["ColorPalette"+configJSON.ColorPalette]["wind"]["g"], configJSON["ColorPalette"+configJSON.ColorPalette]["wind"]["b"])
        //         //$('input[id="DateColorInput"]')[0].value = rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette]["date"]["r"], configJSON["ColorPalette"+configJSON.ColorPalette]["date"]["g"], configJSON["ColorPalette"+configJSON.ColorPalette]["date"]["b"])
        //         //$('input[id="WeatherColorInput"]')[0].value = rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette]["weather"]["r"], configJSON["ColorPalette"+configJSON.ColorPalette]["weather"]["g"], configJSON["ColorPalette"+configJSON.ColorPalette]["weather"]["b"])
        //     }
        // }
        //configUpdating = false;
    }
}

function saveConfig() {
    $('#modalLabel')[0].innerText = 'Saving';
    $('.modal-footer').hide();
    $('.modal').modal('show');
    const apiUrl = '/config';
    var configJSON = $('#ConfigJSON')[0].innerText;
    const requestOptions = {
        method: 'POST',
        headers: {
        'Content-Type': 'application/json',
        },
        body: JSON.stringify(JSON.parse(configJSON)),
    };
    fetch(apiUrl, requestOptions)
    .then(response => {
        $('.modal').modal('hide');
    })
    .catch(error => {
        $('.modal-body')[0].innerHTML = '<p>Save Failed!</p>';
    ('Error:', error);
    });
}

function resetConfig() {
    $('.modal-footer').hide();
    $('#modalLabel')[0].innerText = 'Reseting';
    $('.modal-body')[0].innerHTML = '<div class="spinner-border" role="status" id="spinner"></div>';
    reset = {};
    reset["ConfigReset"] = "true";
    resetJSON = JSON.stringify(reset, null, 4);
    const apiUrl = '/reset';
    const requestOptions = {
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
    },
    body: resetJSON,
    };
    fetch(apiUrl, requestOptions)
    .then(response => {
    setTimeout(function () { }, 20000);
    location.reload();
    })
    .catch(error => {
    $('.modal-body')[0].innerHTML = '<p>Reset Failed!</p>';
    ('Error:', error);
    });
}

function resetModal() {
    $('#modalLabel')[0].innerText = 'Reset Config';
    $('.modal-body')[0].innerHTML = '<p>This will clear the crrent config and restore the default values from the params.h file.</p><br><p>Are you sure you want to continue?</p>';
    $('.modal-footer').show();
    $('.modal').modal('show');
}

$(window).on('load', function(){
    loadConfig();
}); 
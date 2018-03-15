module.exports = [
  {
    "type": "heading",
    "defaultValue": "TimeCube Configuration"
  },
  {
    "type": "text",
    "defaultValue": "TimeCube is a watchface developed by venssy."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "defaultValue": "0x130C0E",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "ForeFontColor",
        "defaultValue": "0x7BBFEA",
        "label": "Fore Font Color"
      },
      {
        "type": "color",
        "messageKey": "SimpleFontColor",
        "defaultValue": "0xFFFFFB",
        "label": "Simple Font Color"
      },
      {
        "type": "color",
        "messageKey": "LowBatteryColor",
        "defaultValue": "0xD15B6C",
        "label": "Low Battery Color"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Weather"
      },
      {
        "type": "input",
        "messageKey": "Location",
        "label": "Input Location:",
        "defaultValue": ""
      },
      {
        "type": "select",
        "messageKey": "TemperatureUnit",
        "label": "Temperature Unit",
        "defaultValue": "Centigrade",
        "options": [
	    {
	      "label": "Centigrade",
	      "value": "Centigrade"
	    },
	    {
	      "label": "Fahrenheit",
	      "value": "Fahrenheit"
	    }
	 ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Notice"
      },
      {
        "type": "toggle",
        "messageKey": "NoticePerForty",
        "label": "Valilate every 40 minitues",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
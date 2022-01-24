# RF-Bridge-EFM8BB1
RF-Bridge-EFM8BB1

This project is based on the portisch firmWare: https://github.com/Portisch/RF-Bridge-EFM8BB1
But the code I copied from halfbackery: https://github.com/Portisch/RF-Bridge-EFM8BB1/commit/97155e1cf77c4687c53d4e3c6dedac1821e8f8bd
Then I included this fix: https://github.com/Portisch/RF-Bridge-EFM8BB1/commit/b741c75ba324a05ad46321789e7a821faf504850#diff-bfc6bf992497244b49a26db2f8a2fa8beeb3379406a1e7ac3f5285b854d2778d
And Added some other fix to make my byron doorbell alsways work.

# RF-Bridge-EFM8BB1
RF-Bridge-EFM8BB1

The Sonoff RF Bridge is only supporting one protocol with 24 bits.<br/>
The Idea is to write a alternative firmware for the onboard EFM8BB1 chip.

Please take a look in the [wiki](https://github.com/Portisch/RF-Bridge-EFM8BB1/wiki) for more information.

# usage example
I use this doorbell but others should work as well.
https://www.bol.com/nl/nl/p/byron-wireless-doorbell-set-wit-werkt-op-batterij-draadloos/9300000008243775/?Referrer=ADVNLGOO002062-G-131329814586-S-1084418633999-9300000008243775&gclid=Cj0KCQiAubmPBhCyARIsAJWNpiOvQZxTuON8yMUkaMCpSxhW1IyF8Izk3rtKrBf0iSkVlATbU0I1NzEaAiGjEALw_wcB

1. folow this manual: https://tasmota.github.io/docs/devices/Sonoff-RF-Bridge-433/
-> but instead use my latest firmware: https://github.com/bkbartk/RF-Bridge-EFM8BB1/releases
2. in tasmota console run: 
```
Rule1 on system#boot do RfRaw 177 endon
```
and
```
Rule1 1
```
to have your device always in raw mode.
3. in HA configuration.yaml add a sensor to grep the 7th part of the signal.

```yaml
sensor:
- platform: mqtt
  name: 433mhz
  state_topic: "tele/tasmota_EDEBDA/RESULT"
  value_template: '{{ value_json["RfRaw"]["Data"].split()[6] }}'
```
4. now you can add an automation based on your grepped signal.
```yaml
- id: Doorbell
  alias: DoorBell
  description: DoorBell
  trigger:
  - platform: state
    entity_id: sensor.433mhz
    to: '281819081819081.....'
  - platform: state
    entity_id: sensor.433mhz
    to: 081819081819081819......
  - platform: state
    entity_id: sensor.433mhz
    to: '2819081909081908190908190908190....'
  - platform: state
    entity_id: sensor.433mhz
    to: 08190819090819081909081909081908....
  action:
  - service: notify.notify
    data:
```
be aware the doorbell sends a few different signals, to be clear I grep them all.
sometimes it sends 2 or 3 so your event is triggered a few times. If you like you can fix this in HA.

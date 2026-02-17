Nexus-Home-LLM-14B
===================

Overview
--------
Nexus-Home-LLM-14B is a prototype smart-home application (app) that demonstrates a minimal end-to-end setup connecting a modern user interface, a cloud realtime store, an edge WiFi module attached to an STM32-based PCB, and an on-premise LLM (14B parameters ANN) for natural language intent processing. This repository is a concise demonstration and not a production system.

Important note
--------------
This project is a technical proof-of-concept. Security, access control, and production-grade deployment practices are intentionally simplified for demonstration purposes. Do not use the default configuration in production.

Demo video & screenshots
------------------------
- Demo video: https://youtu.be/lMc62OIt2-Q

Screenshots (in-repo)
---------------------
Preview images are included in the `screenshots/` folder. Examples:

![Screenshot 1](screenshots/Screenshot%202026-02-18%20at%2001.33.13.png)

![Screenshot 2](screenshots/Screenshot%202026-02-18%20at%2001.33.20.png)

![Screenshot 3](screenshots/Screenshot%202026-02-18%20at%2001.33.28.png)

![Screenshot 4](screenshots/Screenshot%202026-02-18%20at%2001.33.43.png)

What this prototype shows
-------------------------
- A clean, app-like UI for controlling lighting.
- Voice commands (Arabic / English) handled in the browser and routed to the system.
- Realtime state updates via Firebase Realtime Database (used as a lightweight realtime bus).
- Edge control: WiFi module + STM32-based PCB receives commands and actuates devices.
- An LLM (14B ANN) used to interpret and normalize natural language commands before executing actions.

Field test
----------
A remote test was performed: a remote user (located in another country) sent voice commands in Arabic — "ولع النور" and "طفّي النور" — and the system successfully toggled the light on a device located in Istanbul, Turkey. This validates basic end-to-end connectivity and command routing for the prototype.

Repository structure
--------------------
- `Frontend/` — single-page app (HTML/CSS/JS) that implements the UI and voice handling.
- `screenshots/` — screenshots used in this README.

Technical summary
-----------------
- Frontend: HTML/CSS/JavaScript (Web Speech API for voice capture, Firebase client for realtime updates).
- Cloud: Firebase Realtime Database (lightweight realtime state channel used for demo).
- Edge: WiFi module connected to an STM32-based PCB (firmware listens to RTDB paths and toggles outputs).
- AI: On-prem LLM (14B parameters ANN) used to parse intents and map natural sentences to control actions.

Running the prototype locally
----------------------------
1. Open `Frontend/index.html` in a modern browser (Chrome or Edge recommended) to use the Web Speech API.
2. Confirm Firebase configuration is set in the frontend file (replace Firebase config with your project credentials).
3. Create a Firebase Realtime Database (for development only use permissive rules) and note the RTDB URL in the frontend config.
4. Connect an edge device that watches the RTDB path `devices/LED` and toggles the physical output accordingly.

Language support
----------------
The UI supports Arabic and English. Toggling the language switches UI text and voice recognition language (Arabic / en-US). The English voice commands include phrases like "turn on", "turn off", "switch on", "switch off" and synonyms; Arabic phrases are supported for common dialect variants.

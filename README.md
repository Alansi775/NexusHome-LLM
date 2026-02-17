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

Security & production checklist
------------------------------
- Move sensitive keys out of source code (use environment variables or a secure secrets store).
- Add authentication and restrict RTDB rules to authenticated users only.
- Harden device provisioning and secure firmware update paths.
- Evaluate hosting options for the LLM (on-prem vs managed) and ensure model access control and rate limiting.

Suggested GitHub repository name
-------------------------------
Primary recommendation: `Nexus-Home-LLM-14B`

Short description (one-liner for GitHub):
"Prototype Smart Home app integrating IoT edge devices with an on-prem LLM (14B) — demo only."

How to publish to GitHub (quick commands)
----------------------------------------
The commands below assume you already created a repository on GitHub named `Nexus-Home-LLM-14B`.

```bash
git init
git add .
git commit -m "Initial prototype: Nexus-Home-LLM-14B"
git remote add origin git@github.com:YOUR_USERNAME/Nexus-Home-LLM-14B.git
git branch -M main
git push -u origin main
```

Recommended `.gitignore` entries
--------------------------------
Add a `.gitignore` to exclude local config and keys. Example lines:

```
# Firebase local config
frontend/.firebase
*.env

# Node / build artifacts (if you add build tools later)
node_modules/
dist/

# OS files
.DS_Store
```

License
-------
MIT

Final notes
-----------
This repository is written and organized as a showcase for your portfolio. It emphasizes your combined skills in embedded systems, AI (LLM), and app UI development. Keep the demo video and screenshots visible on the repo landing page to make the demo immediate and credible for recruiters and collaborators.

Secure deployment & workflow (what to do before you push)
-------------------------------------------------------
1. Add local secrets (do NOT commit):
	- Copy `.env.example` to `.env` and fill values.
	- Create `Frontend/firebase-config.js` using `firebase-config.example.js` as a template and fill your Firebase project values.
	- Make sure `Frontend/firebase-config.js` is listed in `.gitignore` (it is).

2. If you previously committed API keys into the repo, rotate them now and remove the old commit history if needed.

3. Remove local copy and re-clone safely:
	- To remove your local project folder when you no longer need it locally:

```bash
cd ..
rm -rf SmartHome
```

	- When you want to restore later, clone the GitHub repo and recreate the local secret files:

```bash
git clone git@github.com:YOUR_USERNAME/Nexus-Home-LLM-14B.git
cd Nexus-Home-LLM-14B
cp .env.example .env    # then fill values locally (do not commit)
cp firebase-config.example.js Frontend/firebase-config.js
# Edit Frontend/firebase-config.js and add your real Firebase values (do not commit)
```

4. Verify app startup locally after creating `Frontend/firebase-config.js`. The frontend will prefer `window.firebaseConfig` (from the external file) and otherwise fall back to the inline example.

5. Recommended final check list before pushing:
	- `.gitignore` contains `Frontend/firebase-config.js` and `*.env`.
	- No secret keys remain in tracked files (`git status` / `git diff`).
	- README includes the demo video and screenshots (already added).


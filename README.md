## **This is a "cracked" version of a popular Minecraft launcher that lets you play the game without a Mojang account.**

## This software is not related to MultiMC developers and provided without any warranty. Please don't bomb MultiMC developers if something gets wrong using this launcher.

## Pre-built binaries:
- Windows / Linux / MacOS: Not Avalabile

To update the launcher replace all replaceable files and folders with the newer one from the above link. Do not use the inbuilt option, as that downloads the MultiMC launcher instead (this will be fixed in a future version).

## How to install and use
1. Download pre-built binaries for your system.
2. Unpack them in your desired directory.
3. Launch `TRLauncher`.
4. Go to account settings, and you will be requested to use email and password.
5. As your email use your username, and then type a random password or use ElyBy account.
6. Save it.
7. Now enjoy the Launcher.

In case if you are using MacOS/Linux then additional step is to make a script `UltimMC` executable by using the command `chmod +x UltimMC` in the terminal

To remove the "localhost:nnnn" or "Ely.by" in the minecraft main menu and F3, add `-Dauthlibinjector.noShowServerName` to the java arguments in UltimMC settings.

# Details about the ORIGINAL launcher below:

MultiMC
=======

MultiMC is a custom launcher for Minecraft that focuses on predictability, long term stability and simplicity.

## Development
If you want to contribute, talk to us on [Discord](https://discord.gg/multimc) first.

While blindly submitting PRs is definitely possible, they're not necessarily going to get accepted.

We aren't looking for flashy features, but expanding upon the existing feature set without distruption or endangering future viability of the project is OK.

### Building
If you want to build the launcher yourself, check [BUILD.md](BUILD.md) for build instructions.

### Code formatting
Just follow the existing formatting.

In general, in order of importance:
* Make sure your IDE is not messing up line endings or whitespace and avoid using linters.
* Prefer readability over dogma.
* Keep to the existing formatting.
* Indent with 4 space unless it's in a submodule.
* Keep lists (of arguments, parameters, initializers...) as lists, not paragraphs. It should either read from top to bottom, or left to right. Not both.

## Translations
Translations can be done [on crowdin](https://translate.multimc.org). Please avoid making direct pull requests to the translations repository.

## License
Copyright &copy; 2013-2022 MultiMC Contributors

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this program except in compliance with the License. You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0).

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

## Forking/Redistributing/Custom builds policy
We keep Launcher open source because we think it's important to be able to see the source code for a project like this, and we do so using the Apache license.

The license gives you access to the source MultiMC is build from, but:
- Not the name, logo and other branding.
- Not the API tokens required to talk to services the launcher depends on.

Because of the nature of the agreements required to interact with the Microsoft identity platform, it's impossible for us to continue allowing everyone to build the code as 'MultiMC'. The source code has been debranded and now builds as `DevLauncher` by default.

You must provide your own branding if you want to distribute your own builds.

You will also have to register your own app on Azure to be able to handle Microsoft account logins.

If you decide to fork the project, a mention of its origins in the About dialog and the license is acceptable. However, it should be abundantly clear that the project is a fork *without* implying that you have our blessing.

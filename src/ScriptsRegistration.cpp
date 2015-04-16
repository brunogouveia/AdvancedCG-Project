if (scriptName == "LightScript") {
	script = new LightScript();
}
else { 
	printf("Script %s does not exist.\n", scriptName.c_str());
	script = new Script(); 
}
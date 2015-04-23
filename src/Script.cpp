/*
 *
 *  Created on: Mar, 2015
 *      Author: Bruno Gouveia
 */

#include "Script.h"
#include <scripts/ScriptsRegistration.h>
    
void Script::OnUpdate(){
    // Do nothing
};


Script * Script::GetScriptByName(Transform * transform, std::string scriptName) {
    // Create script
    Script * script;
#include "ScriptsRegistration.cpp"

    // Set transform reference
    script->transform = transform;

    // Return script
    return script;
}
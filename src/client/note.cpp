#include "note.hpp"

NoteUI::NoteUI() {
    noteDisplay = make_unique<BaseUI>(0, 0, 1.0f, 1.0f, 0, false,false);
    noteDisplay->setTexture(BaseUI::loadTexture("../resources/ui/CharacterSelection.png"));
    noteDisplay->setShader(make_unique<Shader>("../resources/shaders/animUi.vert", "../resources/shaders/animUi.frag"));
    closeButton = make_unique<BaseUI>(-0.8f, 0.8f, buttonSize, buttonSize, 1, true, true);
    closeButton->setTexture(BaseUI::loadTexture("../resources/ui/Buttons/sprite_08.png"));
    closeButton->setHoverTexture(BaseUI::loadTexture("../resources/ui/Buttons/sprite_09.png"));
    closeButton->setShader(make_unique<Shader>("../resources/shaders/animUi.vert", "../resources/shaders/animUi.frag"));
    closeButton->setOnClick([this]() {
        if (onCloseCallback) {
            onCloseCallback();
        }
        display = false;
    });
    display = false;
}

void NoteUI::draw() {
    if (display) {
        noteDisplay->draw();
        closeButton->draw();
    }
}

void NoteUI::update(float mouseX, float mouseY, int winWidth, int winHeight,
                    float deltatime) {
    if (display) {
        closeButton->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    }
}

void NoteUI::setObjectID(OBJECT_ID objectId) {
    id = objectId;
}

void NoteUI::setOnClose(function<void()> callback) {
    onCloseCallback = move(callback);
}
/*
 * Copyright 2013-2021 MultiMC Contributors
 * Copyright 2021-2022 kb1000
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ModrinthPage.h"

#include "ui_ModrinthPage.h"

#include <QKeyEvent>

ModrinthPage::ModrinthPage(NewInstanceDialog *dialog, QWidget *parent) : QWidget(parent), ui(new Ui::ModrinthPage), dialog(dialog)
{
    ui->setupUi(this);
}

ModrinthPage::~ModrinthPage()
{
    delete ui;
}

void ModrinthPage::openedImpl()
{
    BasePage::openedImpl();
    triggerSearch();
}

bool ModrinthPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->searchEdit && event->type() == QEvent::KeyPress) {
        auto *keyEvent = reinterpret_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            this->triggerSearch();
            keyEvent->accept();
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void ModrinthPage::triggerSearch() {

}

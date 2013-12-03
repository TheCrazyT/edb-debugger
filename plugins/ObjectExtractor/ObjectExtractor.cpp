#include "ObjectExtractor.h"
#include "DialogObjectExtractor.h"
#include "edb.h"
#include <QMenu>


ObjectExtractor::ObjectExtractor() : menu_(0),dialog_(0) {
}
ObjectExtractor::~ObjectExtractor(){
    delete dialog_;
}

//------------------------------------------------------------------------------
// Name: private_init
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::private_init() {
}

//------------------------------------------------------------------------------
// Name: menu
// Desc:
//------------------------------------------------------------------------------
QMenu *ObjectExtractor::menu(QWidget *parent) {

    Q_ASSERT(parent);

    if(!menu_) {
        menu_ = new QMenu(tr("ObjectExtractor"), parent);
        menu_->addAction (tr("&ObjectExtractor"), this, SLOT(show_menu()), QKeySequence(tr("Ctrl+O")));
    }

    return menu_;
}


//------------------------------------------------------------------------------
// Name: show_menu
// Desc:
//------------------------------------------------------------------------------
void ObjectExtractor::show_menu() {
    if(!dialog_) {
        dialog_ = new DialogObjectExtractor(edb::v1::debugger_ui);
    }

    dialog_->show();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ObjectExtractor, ObjectExtractor)
#endif

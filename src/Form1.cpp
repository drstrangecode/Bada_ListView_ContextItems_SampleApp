 /*
 * Created by: Dr. Strangecode
 * ---------------------------------------------
 * This is free code, it can be used, modified,
 * destroyed, raped and whatever without limits.
 * I own no copyrights over it.
 * This code is provided AS IS without warranty
 * of any kind either expressed or implied,
 * including but not limited to the implied
 * warranties of merchantability and/or fitness
 * for a particular purpose.
 * ---------------------------------------------
 * For more free code visit http://drstrangecode.org
 */

#include "Form1.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::App;

Form1::Form1(void) {
}

Form1::~Form1(void) {
}

bool Form1::Initialize() {
    // Construct an XML form
    Construct(L"IDF_FORM1");

    return true;
}

result Form1::OnInitializing(void) {
    result r = E_SUCCESS;

    pContacts = new ArrayList();
    pContacts->Construct();
    pContacts->Add(*new String(L"Arthur Dent"));
    pContacts->Add(*new String(L"Ford Prefect"));
    pContacts->Add(*new String(L"Zaphod Beeblebrox"));
    pContacts->Add(*new String(L"Vogon Headquarters"));
    pContacts->Add(*new String(L"Marvin"));
    pContacts->Add(*new String(L"Tricia McMillan"));

    AppResource * res = Application::GetInstance()->GetAppResource();
    pCallIcon = res->GetBitmapN(L"ctx_phone_call.png");
    pSmsIcon = res->GetBitmapN(L"ctx_send_sms.png");
    pEmailIcon = res->GetBitmapN(L"ctx_send_email.png");
    pUserPlaceholderIcon = res->GetBitmapN(L"user_placeholder.png");

    pListView = static_cast<ListView *> (GetControl(L"IDC_LISTVIEW"));
    pListView->SetItemProvider(*this);
    pListView->AddListViewItemEventListener(*this);
    pListView->SetSweepEnabled(true);

    return r;
}

result Form1::OnTerminating(void) {
    result r = E_SUCCESS;

    delete pCallIcon;
    delete pSmsIcon;
    delete pEmailIcon;
    delete pUserPlaceholderIcon;

    /* Remove all strings from array and deallocate them */
    pContacts->RemoveAll(true);
    /* Deallocate the array itself */
    delete pContacts;

    return r;
}

// IListViewItemEventListener
void Form1::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView,
                                              int itemIndex,
                                              int elementId,
                                              Osp::Ui::Controls::ListContextItemStatus state) {

	AppLog("OnListViewContextItemStateChanged() : Item: %d", itemIndex);

	String message;

	switch (elementId) {
	case ID_CONTEXT_ITEM_CALL:
		message.Append(L"Calling ");
		break;
	case ID_CONTEXT_ITEM_SMS:
		message.Append(L"Sending an SMS to ");
		break;
	case ID_CONTEXT_ITEM_EMAIL:
		message.Append(L"Sending an e-mail to ");
		break;
	}

	String * contact = static_cast<String *>(pContacts->GetAt(itemIndex));
	message.Append(*contact);
	message.Append(L"...");

    MessageBox messageBox;
    messageBox.Construct(L"Context action", message, MSGBOX_STYLE_OK);

    // Calls ShowAndWait - draw, show itself and process events
    int modalResult = 0;
    messageBox.ShowAndWait(modalResult);

}

void Form1::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView,
                                       int itemIndex,
                                       int elementId,
                                       Osp::Ui::Controls::ListItemStatus state) {

	AppLog("OnListViewItemStateChanged() : Item: %d", itemIndex);
	switch (state) {
		case LIST_ITEM_STATUS_CHECKED:
			AppLog("LIST_ITEM_STATUS_CHECKED");
		break;
		case LIST_ITEM_STATUS_HIGHLIGHTED:
			AppLog("LIST_ITEM_STATUS_HIGHLIGHTED");
		break;
		case LIST_ITEM_STATUS_MORE:
			AppLog("LIST_ITEM_STATUS_MORE");
		break;
		case LIST_ITEM_STATUS_SELECTED:
			AppLog("LIST_ITEM_STATUS_SELECTED");
		break;
		case LIST_ITEM_STATUS_UNCHECKED:
			AppLog("LIST_ITEM_STATUS_UNCHECKED");
		break;
	}
}

void Form1::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView,
                                int itemIndex,
                                Osp::Ui::Controls::SweepDirection direction) {

	AppLog("OnListViewItemSwept() : Item: %d", itemIndex);
	switch (direction) {
	case SWEEP_DIRECTION_LEFT:
		AppLog("SWEEP_DIRECTION_LEFT");
	break;
	case SWEEP_DIRECTION_RIGHT:
		AppLog("SWEEP_DIRECTION_RIGHT");
	break;
	}
}

void Form1::OnListViewItemLongPressed(Osp::Ui::Controls::ListView & listView,
                                             int  itemIndex,
                                             int  elementId,
                                             bool & invokeListViewItemCallback) {

	AppLog("OnListViewItemLongPressed() : Item: %d", itemIndex);

}

// IListViewItemProvider
int Form1::GetItemCount() {
    return pContacts->GetCount();
}

Osp::Ui::Controls::ListItemBase* Form1::CreateItem(int itemIndex, int itemWidth) {

    String * contact = static_cast<String *>(pContacts->GetAt(itemIndex));

	CustomItem * pItem = new CustomItem();

	int nameHeight = LIST_ITEM_HEIGHT*0.6;
	int commentHeight = LIST_ITEM_HEIGHT*0.4;

	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_NORMAL);
	pItem->AddElement(Rectangle(0, 0, pUserPlaceholderIcon->GetWidth(), pUserPlaceholderIcon->GetHeight()), 0, *pUserPlaceholderIcon, pUserPlaceholderIcon, pUserPlaceholderIcon);
	pItem->AddElement(Rectangle(pUserPlaceholderIcon->GetWidth(), 0, itemWidth, nameHeight), 1, *contact, nameHeight, Color::COLOR_WHITE, Color::COLOR_BLACK, Color::COLOR_BLACK, false);
	pItem->AddElement(Rectangle(pUserPlaceholderIcon->GetWidth(), nameHeight, itemWidth, commentHeight), 2, L"Slide for more actions", commentHeight, Color::COLOR_GREY, Color::COLOR_BLACK, Color::COLOR_BLACK, false);

	pItem->SetElementTextVerticalAlignment(1, ALIGNMENT_MIDDLE);
	pItem->SetElementTextVerticalAlignment(2, ALIGNMENT_MIDDLE);

	ListContextItem* pItemContext = new ListContextItem();
	pItemContext->Construct();
	pItemContext->AddElement(ID_CONTEXT_ITEM_CALL, L"Call", *pCallIcon, *pCallIcon, pCallIcon, true);
	pItemContext->AddElement(ID_CONTEXT_ITEM_SMS, L"SMS", *pSmsIcon, *pSmsIcon, pSmsIcon, true);
	pItemContext->AddElement(ID_CONTEXT_ITEM_EMAIL, L"E-mail", *pEmailIcon, *pEmailIcon, pEmailIcon, true);

	pItem->SetContextItem(pItemContext);

    return pItem;

}

bool Form1::DeleteItem(int itemIndex,
                       Osp::Ui::Controls::ListItemBase* pItem,
                       int itemWidth) {
	/* pItem is not deleted here */
	return false;
}

#include "gptapikeyreview.h"
#include "ui_gptapikeyreview.h"

GptApiKeyReview::GptApiKeyReview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GptApiKeyReview)
{
    ui->setupUi(this);

    // 초기값: 각 컬럼은 오름차순으로 정렬
    sortOrderApiKey  = new bool(true);
    sortOrderRegDate = new bool(true);
    sortOrderRegTime = new bool(true);

    ui->wGptApiKeyList->setColumnCount(3);                                                                 // 열의 이름 설정
    ui->wGptApiKeyList->setHorizontalHeaderLabels(QStringList() << "API Key" << "등록일자" << "등록 시간");    // API Key, 등록일자, 등록시간 순으로 컬럼 설정
    ui->wGptApiKeyList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);                    // 컬럼이 테이블 전체 크기에 맞춰 늘어나도록 설정

    connect(ui->wGptApiKeyList->horizontalHeader(), &QHeaderView::sectionClicked, this, &GptApiKeyReview::sortByColumn);        // 각 컬럼 헤더 클릭 시 정렬을 처리하는 슬롯 연결
    connect(ui->checkBox,     &QCheckBox::stateChanged, this,   &GptApiKeyReview::onCheckBoxStateChanged);                      // 체크박스 상태 변경 시 호출되는 함수 연결
    connect(ui->addButton,    &QPushButton::clicked,    this,   &GptApiKeyReview::onAddButtonClicked);                          // 추가 버튼 클릭 시 onAddButtonClicked 함수 실행
    connect(ui->deleteButton, &QPushButton::clicked,    this,   &GptApiKeyReview::onDeleteButtonClicked);                       // 삭제 버튼 클릭 시 onDeleteButtonClicked 함수 실행
}

GptApiKeyReview::~GptApiKeyReview()
{
    delete sortOrderApiKey;
    delete sortOrderRegDate;
    delete sortOrderRegTime;
    delete ui;
}

void GptApiKeyReview::onAddButtonClicked()              // 추가 버튼 클릭 시 실행
{
    QString apiKey = ui->apiKeyLineEdit->text();        // API Key 입력값 가져오기
    if(apiKey.isEmpty())
        return;         // 빈 값이면 아무 작업도 하지 않음

    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");  // 현재 날짜 및 시간 가져오기
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");

    int newRow = ui->wGptApiKeyList->rowCount();    // 새로운 행 추가
    ui->wGptApiKeyList->insertRow(newRow);

    QTableWidgetItem *keyItem = new QTableWidgetItem(apiKey);           // 각 열에 데이터를 설정
    QTableWidgetItem *dateItem = new QTableWidgetItem(currentDate);
    QTableWidgetItem *timeItem = new QTableWidgetItem(currentTime);

    ui->wGptApiKeyList->setItem(newRow, 0, keyItem);
    ui->wGptApiKeyList->setItem(newRow, 1, dateItem);
    ui->wGptApiKeyList->setItem(newRow, 2, timeItem);

    ui->apiKeyLineEdit->clear();                                        // 입력란을 비우고 포커스를 다시 설정
    ui->apiKeyLineEdit->setFocus();
}

// 삭제 버튼 클릭 시 실행
void GptApiKeyReview::onDeleteButtonClicked()
{
    QList<QTableWidgetItem *> selectedItems = ui->wGptApiKeyList->selectedItems();          // 선택된 항목들의 인덱스를 가져옴

    if (selectedItems.isEmpty())                                            // 선택된 항목이 없는 경우, 함수 종료
        return;

    // 선택된 항목들의 행을 삭제
    QList<int> selectedRows;
    for (QTableWidgetItem *item : selectedItems)
    {
        int row = item->row();
        if (!selectedRows.contains(row))
            selectedRows.append(row);
    }

    // 역순으로 삭제하여 인덱스가 꼬이지 않도록 처리
    std::sort(selectedRows.begin(), selectedRows.end(), std::greater<int>());
    for (int row : selectedRows) {
        ui->wGptApiKeyList->removeRow(row);
    }
}

// 컬럼에 맞는 정렬 상태 포인터 반환
bool* GptApiKeyReview::getSortOrderPointer(int column)
{
    switch (column)
    {
    case 0: return sortOrderApiKey;   // API Key 컬럼
    case 1: return sortOrderRegDate;  // 등록 일자 컬럼
    case 2: return sortOrderRegTime;  // 등록 시간 컬럼
    default: return nullptr;
    }
}

void GptApiKeyReview::sortByColumn(int column)          // 컬럼별 정렬을 처리
{
    bool* sortOrder = getSortOrderPointer(column);
    if (sortOrder == nullptr) return;

    Qt::SortOrder order = *sortOrder ? Qt::AscendingOrder : Qt::DescendingOrder;    // 오름차순/내림차순 토글
    ui->wGptApiKeyList->sortItems(column, order);

    *sortOrder = !(*sortOrder);    // 정렬 상태 반전
}

void GptApiKeyReview::onCheckBoxStateChanged(int state)     // 체크박스 상태 변경 시 호출
{
    if (state == Qt::Checked)
        ui->wGptApiKeyList->setEnabled(false);              // 체크박스가 체크되면 wGptApiKeyList를 비활성화
    else
        ui->wGptApiKeyList->setEnabled(true);               // 체크박스가 체크 해제되면 wGptApiKeyList를 활성화
}

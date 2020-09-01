#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("FATool (v " + VERSION + ")");
    setFixedSize(this->width(), this->height());

    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(onLoadClick()));
    connect(ui->goButton, SIGNAL(clicked()), this, SLOT(onGoClick()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(onClearClick()));
    connect(ui->quitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels({"File name", "Vector name", "Vector type"});
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(openContextMenu(const QPoint&)));
    connect(ui->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(fillList()));

    connect(this, SIGNAL(allFilesLoaded()), this, SLOT(afterAllFilesLoaded()));

    instrWindow();

    dataMap = new FilesData();
}

MainWindow::~MainWindow()
{
    delete dataMap;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event->Close)
        QApplication::quit();
}

void MainWindow::instrWindow()
{
    QFile f(prefsFile);
    QTextStream in(&f);
    QString str = hideStartWin;
    if(f.open(QFile::ReadWrite)){
        str = in.readLine();
        f.close();
    }
    if(str == showStartWin)
        startWin = new StartingWindow();
}

void MainWindow::onLoadClick()
{
    fileNames.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(dataFilter);
    if(dialog.exec())
        fileNames.append(dialog.selectedFiles());

    QStringList alreadyLoadedFiles;
    for(QString f : fileNames)
    {
        if(dataMap->getKeys().contains(f))
            alreadyLoadedFiles.append(f);
    }
    for(QString f : alreadyLoadedFiles)
        fileNames.removeOne(f);

    if(alreadyLoadedFiles.size() > 0)
    {
        QString w = "These files have already been loaded and will be ignored, you can modify them from the table.\n -";
        w.append(alreadyLoadedFiles.join("\n- "));
        QMessageBox messageBox;
        messageBox.warning(nullptr, "Warning", w);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }

    if(fileNames.size() > 0)
    {
        LoadFilesWindow *fWin;
        if(fileNames.size() > 1)
            fWin = new LoadFilesWindow(QString::number(fileNames.size()) + " files");
        else
            fWin = new LoadFilesWindow(fileNames.at(0));
        connect(fWin, SIGNAL(filesSpecsInserted(QString, QString, std::map<QString, std::pair<QString, QString>>)),
                this, SLOT(onFilesSpecsInserted(QString, QString, std::map<QString, std::pair<QString, QString>>)));
        fWin->show();
    }
}

void MainWindow::onFilesSpecsInserted(QString del, QString header, std::map<QString, std::pair<QString, QString>> map)
{
    ui->statusLabel->setText("Loading files...");
    QApplication::processEvents();

    auto dataFuture = std::async(std::launch::async, &FilesData::setDataMap, dataMap, fileNames, del, header, map);
    while(dataFuture.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
    {
        updateFilesTable();
        QApplication::processEvents();
    }

    updateFilesTable();
    onMapReady();
}

void MainWindow::updateFilesTable()
{
    clearFilesTable();

    dataMap->lock();
    std::map<QString, DataFile*> mapForTable = dataMap->getDataMap();
    dataMap->unlock();

    for(auto const& [key, val] : mapForTable)
    {
        for(int col : val->getColumns())
        {
            int xCol = val->getXAxisColumn();
            if(col != xCol)
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0,
                                         new QTableWidgetItem(key));
                if(xCol != 0)
                {
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1,
                                             new QTableWidgetItem(val->getNameOfColumn(xCol) +
                                                                  " (column " + QString::number(xCol) + ")\n" +
                                                                  val->getNameOfColumn(col) +
                                                                  " (column " + QString::number(col) + ")"));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2,
                                             new QTableWidgetItem(val->getTypeOfColumn(xCol) +
                                                                  "\n" +
                                                                  val->getTypeOfColumn(col)));
                }
                else
                {
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1,
                                             new QTableWidgetItem(val->getNameOfColumn(col) +
                                                                  " (column " + QString::number(col) + ")"));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2,
                                             new QTableWidgetItem(val->getTypeOfColumn(col)));
                }
            }
        }
        ui->tableWidget->update();
    }
}

void MainWindow::onMapReady()
{
    ui->statusLabel->setText("All files loaded");

    std::thread statusThread([this](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        emit allFilesLoaded();
    });
    statusThread.detach();
}

void MainWindow::afterAllFilesLoaded()
{
    ui->statusLabel->setText("");
}

void MainWindow::clearFilesTable()
{
    ui->tableWidget->setRowCount(0);
}

void MainWindow::openContextMenu(const QPoint& pos)
{
    QMenu menu(ui->tableWidget);
    QAction singlePlotAction("Single plot");
    QAction multiplePlotsAction("Multiple plots");
    QAction deleteAction("Delete");
    QAction modifyAction("Modifiy");

    menu.addAction(&singlePlotAction);
    if(ui->tableWidget->selectionModel()->selectedRows().size() > 1)
        menu.addAction(&multiplePlotsAction);
    menu.addAction(&deleteAction);
    menu.addAction(&modifyAction);

    connect(&singlePlotAction, SIGNAL(triggered()), this, SLOT(singlePlot()));
    connect(&multiplePlotsAction, SIGNAL(triggered()), this, SLOT(multiplePlots()));
    connect(&deleteAction, SIGNAL(triggered()), this, SLOT(deleteRows()));
    connect(&modifyAction, SIGNAL(triggered()), this, SLOT(updateRows()));

    menu.exec(mapToGlobal(pos));
}

void MainWindow::singlePlot()
{
    QApplication::processEvents();

    QModelIndexList idxs = ui->tableWidget->selectionModel()->selectedRows(0);
    QStringList types;
    std::vector<int> cols(idxs.size());
    std::vector<DataFile*> data(idxs.size());

    for(int i = 0; i < int(idxs.size()); i++)
    {
        QModelIndex idx = idxs.at(i);
        data.at(i) = dataMap->getDataMap().at(ui->tableWidget->item(idx.row(), 0)->text());
        cols.at(i) = ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split(" ").last().front().digitValue();
        types.append(ui->tableWidget->item(idx.row(), 2)->text().split("\n").last());
    }

    if((types.contains(yVec)) && (types.contains(flucVec)))
    {
        QMessageBox messageBox;
        QString errToShow = "Cannot plot fluctuations and series together.";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
    else
    {
        if(types.at(0) == yVec)
        {
            DataPlotWindow *plot = new DataPlotWindow(data, cols);
            plot->show();
        }
        else if(types.at(0) == flucVec)
        {
            // TODO: altra finestra con più opzioni se voglio modificare i fit, fit multipli,
            // scale log (anche su più fluttuazioni nello stesso grafico)
        }
    }
}

void MainWindow::multiplePlots()
{
    QApplication::processEvents();

    QModelIndexList idxs = ui->tableWidget->selectionModel()->selectedRows(0);

    for(int i = 0; i < int(idxs.size()); i++)
    {
        QModelIndex idx = idxs.at(i);
        DataFile *data = dataMap->getDataMap().at(ui->tableWidget->item(idx.row(), 0)->text());
        int col = ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split(" ").last().front().digitValue();
        QString type = ui->tableWidget->item(idx.row(), 2)->text().split("\n").last();

        if(type == yVec)
        {
            DataPlotWindow *plot = new DataPlotWindow(std::vector<DataFile*> {data}, std::vector<int> {col});
            plot->show();
        }
        else if(type == flucVec)
        {
            // TODO: altra finestra con più opzioni se voglio modificare i fit, fit multipli,
            // scale log (anche su più fluttuazioni nello stesso grafico)
        }
    }
}

void MainWindow::deleteRows()
{
    QModelIndexList idxs = ui->tableWidget->selectionModel()->selectedRows(0);
    std::vector<std::pair<QString, int>> toDelete = std::vector<std::pair<QString, int>>();
    for(int i = 0; i < int(idxs.size()); i++)
    {
        QModelIndex idx = idxs.at(i);
        QString data = ui->tableWidget->item(idx.row(), 0)->text();
        int col = ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split(" ").last().front().digitValue();
        toDelete.push_back(std::pair<QString, int>(data, col));
    }
    dataMap->deleteFromMap(toDelete);
    updateFilesTable();
}

void MainWindow::updateRows()
{
    QModelIndexList idxs = ui->tableWidget->selectionModel()->selectedRows(0);
    QStringList files, cols, names;
    for(int i = 0; i < int(idxs.size()); i++)
    {
        QModelIndex idx = idxs.at(i);
        files.append(ui->tableWidget->item(idx.row(), 0)->text());
        cols.append(QString::number(ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split(" ").last().front().digitValue()));
        names.append(ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split("(").first().trimmed());
    }
    UpdateTableWidget *w = new UpdateTableWidget(files, cols, names, "Modify records");
    connect(w, SIGNAL(newTableValues(QStringList, QStringList, QStringList)),
            this, SLOT(onTableModified(QStringList, QStringList, QStringList)));
    w->show();
}

void MainWindow::onTableModified(QStringList f, QStringList c, QStringList n)
{
    for(int i = 0; i < int(f.size()); i++)
        dataMap->getDataMap().at(f.at(i))->setNameOfColumn(c.at(i).toInt(), n.at(i));

    updateFilesTable();
}

void MainWindow::fillList()
{
    QModelIndexList rows = ui->tableWidget->selectionModel()->selectedRows(0);
    ui->ddList->clear();
    ui->ddList->addItem("-");
    if(rows.size() > 0)
    {
        ui->ddList->addItem(strDFA);
        ui->ddList->addItem(strMFDFA);
        ui->ddList->addItem(strHT);
        if(rows.size() > 1)
        {
            ui->ddList->addItem(strDCCA);
            ui->ddList->addItem(strRHODCCA);
        }

        ui->goButton->setEnabled(true);
    }
    else
    {
        ui->goButton->setEnabled(false);
    }
}

void MainWindow::onGoClick()
{
    QString analysisType = ui->ddList->currentText();
    QModelIndexList idxs = ui->tableWidget->selectionModel()->selectedRows(0);
    QStringList files, cols, names;
    bool ok = true;
    for(int i = 0; i < int(idxs.size()); i++)
    {
        QModelIndex idx = idxs.at(i);
        files.append(ui->tableWidget->item(idx.row(), 0)->text());
        cols.append(QString::number(ui->tableWidget->item(idx.row(), 1)->text().split("\n").last().split(" ").last().front().digitValue()));

        QString type = ui->tableWidget->item(idx.row(), 2)->text().split("\n").last();
        if(type == flucVec)
        {
            ok = false;

            QMessageBox messageBox;
            QString errToShow = "Cannot analyze a fluctuation vector.";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);

            break;
        }
    }

    if(ok && (analysisType != "-"))
    {
        if(analysisType == strDFA)
        {
            InputsDFA *dfaInptWin = new InputsDFA(files, cols, dataMap);
            dfaInptWin->show();
            connect(dfaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)), this, SLOT(onCloseInputWin(std::vector<FA*>)));//, Qt::QueuedConnection);
        }
        else if(analysisType == strDCCA)
        {
            InputsDCCA *dccaInptWin = new InputsDCCA(files, cols, dataMap);
            dccaInptWin->show();
            connect(dccaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)), this, SLOT(onCloseInputWin(std::vector<FA*>))); //, Qt::QueuedConnection);
        }
        else if(analysisType == strMFDFA)
        {
            InputsMFDFA *mfdfaInptWin = new InputsMFDFA(files, cols, dataMap);
            mfdfaInptWin->show();
            connect(mfdfaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)), this, SLOT(onCloseInputWin(std::vector<FA*>))); //, Qt::QueuedConnection);
        }
        else if(analysisType == strRHODCCA)
        {
            InputsrhoDCCA *rhodccaInptWin = new InputsrhoDCCA(files, cols, dataMap);
            rhodccaInptWin->show();
            connect(rhodccaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)), this, SLOT(onCloseInputWin(std::vector<FA*>))); //, Qt::QueuedConnection);
        }
        else if(analysisType == strHT)
        {
            InputsHT *htInptWin = new InputsHT(files, cols, dataMap);
            htInptWin->show();
            connect(htInptWin, SIGNAL(inputsInserted(std::vector<FA*>)), this, SLOT(onCloseInputWin(std::vector<FA*>))); //, Qt::QueuedConnection);
        }
    }
}

void MainWindow::onCloseInputWin(std::vector<FA*> fa)
{
    QString algo = fa.at(0)->getAlgorithmStr();

    for(int i = 0; i < int(fa.size()); i++)
    {
        if(algo != strRHODCCA)
            fa.at(i)->setVectors();
        bool execStop = fa.at(i)->executeAlgorithm();
        if(!execStop)
        {
            if((algo == strDFA) || (algo == strDCCA))
                fa.at(i)->executeFit(fa.at(i)->getMinWin(), fa.at(i)->getMaxWin());

            if(algo == strRHODCCA)
            {
                rhoDCCA *obj = (rhoDCCA *)fa.at(i);
                if(obj->threshCompute())
                    obj->computeThresholds();
            }

            //DFAWindow *plotWin = new DFAWindow(obj);
            //plotWin->setAttribute(Qt::WA_DeleteOnClose);
            //plotWin->show();
        }
    }
}

/*void MainWindow::onCloseDCCAInputWin(std::vector<FA*> dcca)
{
    MathOps mo;
    long long int combs = mo.binCoeff(fileNames.size(), 2);
    for(int i = 0;  i < combs; i++)
    {
        DCCA *obj = (DCCA *)dcca[i];
        obj->setVectors();
        bool execStop = obj->computeFlucVec();
        if(!execStop)
        {
            obj->fitFlucVec(obj->getMinWin(), obj->getMaxWin());
            DCCAWindow *plotWin = new DCCAWindow(obj);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}*/

/*void MainWindow::onCloseMFDFAInputWin(std::vector<FA*> mfdfa)
{
    for(int i = 0;  i < fileNames.size(); i++)
    {
        MFDFA *obj = (MFDFA *)mfdfa[i];
        obj->setVectors();
        bool execStop = obj->computeFlucVec();
        if(!execStop)
        {
            MFDFAWindow *plotWin = new MFDFAWindow(obj);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}*/

/*void MainWindow::onCloseRHODCCAInputWin(std::vector<FA*> rhodcca)
{
    MathOps mo;
    long long int combs = mo.binCoeff(fileNames.size(), 2);
    for(int i = 0;  i < combs; i++)
    {
        rhoDCCA *obj = (rhoDCCA *)rhodcca[i];
        bool execStop = obj->computeRho();
        if(!execStop)
        {
            if(obj->threshCompute())
                obj->computeThresholds();
            rhoDCCAWindow *plotWin = new rhoDCCAWindow(obj);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}*/

/*void MainWindow::onCloseHTInputWin(std::vector<FA*> ht)
{
    for(int i = 0;  i < fileNames.size(); i++)
    {
        HT *obj = (HT *)ht[i];
        obj->setVectors();
        bool execStop = obj->computeFlucVec();
        if(!execStop)
        {
            HTWindow *plotWin = new HTWindow(obj);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}*/

void MainWindow::onClearClick()
{
    clearFilesTable();
    dataMap->clearMap();
}

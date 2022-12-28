#include "map_select_dlg.h"
#include "ui_map_select_dlg.h"

MapSelectDlg::MapSelectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapSelectDlg)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    model_ = new QStandardItemModel();
    ui->tableView->setModel(model_);

    model_->setColumnCount(4);
    model_->setHeaderData(0, Qt::Horizontal, "序号");
    model_->setHeaderData(1, Qt::Horizontal, "地图ID");
    model_->setHeaderData(2, Qt::Horizontal, "地图描述");
    model_->setHeaderData(3, Qt::Horizontal, "存放位置");

    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setColumnWidth(1, 180);
    ui->tableView->setColumnWidth(2, 250);

    if (g_common_info_.business_status.map_id.length() > 0)
    {
        int indexPos = 0;
        QStandardItem *item = new QStandardItem;  
        item->setTextAlignment(Qt::AlignCenter);       
        item->setText(QString("%1").arg(indexPos + 1)); 
        model_->setItem(indexPos, 0, item);

        item = new QStandardItem;  
        item->setTextAlignment(Qt::AlignCenter);     
        item->setText(QString::fromStdString(g_common_info_.business_status.map_id)); 
        model_->setItem(indexPos, 1, item);

        item = new QStandardItem;  
        item->setTextAlignment(Qt::AlignCenter);     
        item->setText(QString::fromStdString(g_common_info_.business_status.map_id)); 
        model_->setItem(indexPos, 2, item);

        item = new QStandardItem;  
        item->setTextAlignment(Qt::AlignCenter);     
        item->setText("车端"); 
        model_->setItem(indexPos, 3, item);
    }

    ui->map_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    map_list_model_ = new QStandardItemModel();
    ui->map_table_view->setModel(map_list_model_);

    map_list_model_->setColumnCount(4);
    map_list_model_->setHeaderData(0, Qt::Horizontal, "序号");
    map_list_model_->setHeaderData(1, Qt::Horizontal, "地图ID");
    map_list_model_->setHeaderData(2, Qt::Horizontal, "地图描述");
    map_list_model_->setHeaderData(3, Qt::Horizontal, "创建日期");

    ui->map_table_view->setColumnWidth(0, 50);
    ui->map_table_view->setColumnWidth(1, 180);
    ui->map_table_view->setColumnWidth(2, 250);

    map_xml_file_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/../map_file/map.xml");
    map_file_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/../map_file/");    
    config_file_path = ros::package::getPath("avos_monitor")+"/config/config.yaml";

    OnLoadConfigParams();
    ui->vehicle_map_path_edit->setText(QString::fromStdString(vehicle_map_path_)); 

    GetDefaultMapID(map_xml_file_path_);
    GetAllFileFolder(map_file_path_);
}

MapSelectDlg::~MapSelectDlg()
{
    delete ui;
    delete model_;
}

void MapSelectDlg::closeEvent(QCloseEvent *event)
{
    OnSaveConfigParams();
}


void MapSelectDlg::on_load_map_btn_clicked()
{
    if (model_->data(model_->index(ui->tableView->currentIndex().row(),3)).toString() != "车端")
    {
        QMessageBox::information(this, "信息提示", "请选择车端地图再加载！");
        return;
    }
    select_map_id_ = model_->data(model_->index(ui->tableView->currentIndex().row(),1)).toString();
    qDebug()  << "车端:select_map_id_" << select_map_id_;

    if (CheckExistMapID(select_map_id_))
    {
        QMessageBox::information(this, "信息提示", "选择的车端地图已存在，切换默认地图即可！");
        return;
    }

    if (select_map_id_.length() > 10)
    {
        std::shared_ptr<std::thread> func_progress_ = std::make_shared<std::thread>(std::bind(&MapSelectDlg::CopyMapfileFromVehicle, this));
        func_progress_->join();
    }
    else
    {
        QMessageBox::information(this, "信息提示", "地图ID为空，请重新选择！");
        return;
    }

    if(!ModifyMapID(map_xml_file_path_, select_map_id_))
    {
        QMessageBox::information(this, "信息提示", "修改本地map.xml中的地图ID失败！");
        return;
    }   

    current_map_id_ = select_map_id_.toStdString();
    return QDialog::accept(); 
}

void MapSelectDlg::on_select_accept_btn_clicked()
{    
    default_map_id_ = map_list_model_->data(map_list_model_->index(ui->map_table_view->currentIndex().row(),1)).toString();
    qDebug()  << "default_map_id_:" << default_map_id_;

    if (default_map_id_.length() < 10)
    {
        QMessageBox::information(this, "信息提示", "请在本地地图列表中选择要切换的地图ID.");
        return;
    }

    if(!ModifyMapID(map_xml_file_path_, default_map_id_))
    {
        QMessageBox::information(this, "信息提示", "修改本地map.xml中的地图ID失败！");
        return;
    }   

    current_map_id_ = default_map_id_.toStdString();

    return QDialog::accept(); 
}


void MapSelectDlg::on_load_default_map_btn_clicked()
{
    return QDialog::accept();
}

//拷贝文件：
bool MapSelectDlg::CopyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if
 
    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}
 
//拷贝文件夹：
bool MapSelectDlg::CopyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    qDebug() << "CopyDirectoryFiles:" << fromDir << toDir;

    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath())) {
            return false;
        }
    }
 
    QFileInfoList fileInfoList = sourceDir.entryInfoList();

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
 
        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!CopyDirectoryFiles(fileInfo.filePath(), 
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName()); 
            }
 
            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(), 
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}

bool MapSelectDlg::OpenXmlFile(QString FilePath)
{
    QFile file( FilePath );
    if( !file.open( QFile::ReadOnly | QFile::Text  ) )
    {
        qDebug() << QObject::tr("error::ParserXML->OpenXmlFile->file.open->%s\n") << FilePath;

        return false;
    }

    if( !m_doc.setContent( &file ) )
    {
        qDebug() << QObject::tr("error::ParserXML->OpenXmlFile->doc.setContent\n") << FilePath;

        file.close();
        return false;
    }
    file.close();
    return true;
}

bool MapSelectDlg::ModifyMapID(QString map_xml_file_path, QString map_id)
{
    if(!OpenXmlFile(map_xml_file_path))
    {
        QMessageBox::warning(this, "警告", "打开map.xml文件失败！");
        return false;
    }
        //修改保存xml
    QDomElement root = m_doc.documentElement();
    if(root.tagName()!= "Map")
    {
        QMessageBox::warning(this, "警告", "解析不到map.xml文件关键字段！");
        return false;
    }
    QDomNode dom_node = root.firstChild();
    while ( !dom_node.isNull() )
    {
        QDomElement element = dom_node.toElement();
        if(!element.isNull())
        {
            if( element.nodeName() == "header" )
            {
                element.setAttribute("default_name", map_id);
            }
        }
        dom_node = dom_node.nextSibling();
    }
    QFile filexml(map_xml_file_path);
    if( !filexml.open( QFile::WriteOnly | QFile::Truncate) ){
        QMessageBox::warning(this, "警告", "写入MapID失败！");
        return false;
    }
    QTextStream ts(&filexml);
    ts.reset();
    ts.setCodec("utf-8");
    m_doc.save(ts, 4, QDomNode::EncodingFromTextStream);
    filexml.close();
    return true; 
}

void MapSelectDlg::CopyMapfileFromVehicle()
{
    // string v_bag_path = "idriver@192.168.1.101:/work/share/project/resident/src/map_file/";//vehicle_map_path_
    string cmd = "sshpass -p idriver scp -r " + vehicle_map_path_  + select_map_id_.toStdString() + " " + map_file_path_.toStdString();
    ROS_INFO("start copy map file cmd: %s", cmd.c_str());

    if(system(cmd.c_str()) != 0)
    {
        ROS_INFO("scp map file failed.");
    } 
    else
    {
        ROS_INFO("scp map file successfully.");
    }
}

void MapSelectDlg::GetAllFileFolder(QString dirPath) 
{
    if (dirPath.isEmpty() || !QDir().exists(dirPath))//是否传入了空的路径||路径是否存在
    {
        QMessageBox::information(this, "信息提示", "请选择正确的路径！");
        return;
    }

    QDir dir(dirPath); 
    dir.setFilter(QDir::Dirs); 
    foreach(QFileInfo fullDir, dir.entryInfoList()) 
    { 
        if(fullDir.fileName() == "." || fullDir.fileName() == "..") continue;         
            GetMapInfo(fullDir.absoluteFilePath()+"/mapinfo.xml");
    } 
}

bool MapSelectDlg::CheckExistMapID(QString map_id)
{
    QDir dir(map_file_path_); 
    dir.setFilter(QDir::Dirs); 
    foreach(QFileInfo fullDir, dir.entryInfoList()) 
    { 
        if(fullDir.fileName() == "." || fullDir.fileName() == "..") continue;

        if (map_id == fullDir.fileName())
        {
            return true;
        }
    } 
    return false;
}

bool MapSelectDlg::GetMapInfo(QString map_info_file_path)
{
    if (map_info_file_path.isEmpty() || !QDir().exists(map_info_file_path))//是否传入了空的路径||路径是否存在
    {
        QMessageBox::information(this, "信息提示", "请选择正确的路径！");
        return false;
    }

    QFile file(map_info_file_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
        file.close();
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if(root.tagName()!= "Map")
    {
        ROS_INFO("GetMapInfo:解析不到mapinfo.xml文件Map关键字段！");
        return false;
    }
    QDomNode dom_node = root.firstChild();
    int indexPos = 0;
    while ( !dom_node.isNull() )
    {
        QDomElement element = dom_node.toElement();
        if(!element.isNull())
        {
            if(element.nodeName() == "header")
            {
                indexPos = map_list_model_->rowCount();
                QStandardItem *item = new QStandardItem;  
                item->setTextAlignment(Qt::AlignCenter);       
                item->setText(QString("%1").arg(indexPos + 1)); 
                map_list_model_->setItem(indexPos, 0, item);

                item = new QStandardItem;  
                item->setTextAlignment(Qt::AlignCenter);     
                item->setText(dom_node.toElement().attribute("name")); 
                map_list_model_->setItem(indexPos, 1, item);

                if (default_map_id_ == dom_node.toElement().attribute("name"))
                {
                    item->setBackground(QColor(0,150,0));
                }

                item = new QStandardItem;  
                item->setTextAlignment(Qt::AlignCenter);     
                item->setText(dom_node.toElement().attribute("describe")); 
                map_list_model_->setItem(indexPos, 2, item);

                item = new QStandardItem;  
                item->setTextAlignment(Qt::AlignCenter);     
                item->setText(dom_node.toElement().attribute("date")); 
                map_list_model_->setItem(indexPos, 3, item);
            }
        }
        dom_node = dom_node.nextSibling();
    }
    return true;
}

bool MapSelectDlg::GetDefaultMapID(QString map_xml_file_path)
{
    if (map_xml_file_path.isEmpty() || !QDir().exists(map_xml_file_path))//是否传入了空的路径||路径是否存在
    {
        QMessageBox::information(this, "信息提示", "请选择正确xml文件路径！");
        return false;
    }

    QFile file(map_xml_file_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
        file.close();
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if(root.tagName()!= "Map")
    {
        ROS_INFO("GetDefaultMapID:解析不到map.xml文件Map关键字段！");
        return false;
    }
    QDomNode dom_node = root.firstChild();
    while ( !dom_node.isNull() )
    {
        QDomElement element = dom_node.toElement();
        if(!element.isNull())
        {
            if(element.nodeName() == "header")
            {
                default_map_id_ = dom_node.toElement().attribute("default_name");
                current_map_id_ = default_map_id_.toStdString();
            }
        }
        dom_node = dom_node.nextSibling();
    }
    return true;

}

/**********************************************
 * Function: OnLoadLocalizationParams()
 * Author: zengzicheng
 * Date: 2020-01-08
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void MapSelectDlg::OnLoadConfigParams()
{    
    try
    {
        YAML::Node params = YAML::LoadFile(config_file_path.c_str());
        YAML::Node params_root = params["map_select"];

        select_map_path_ = params_root["select_map_path"].as<string>();
        vehicle_map_path_ = params_root["vehicle_map_path"].as<string>();
    }
    catch(...)
    {
    }
}

/**********************************************
 * Function: OnSaveLocalizationParams()
 * Author: zengzicheng
 * Date: 2020-01-08
 * Description:
 * Input: None
 * Output: None
 * Return: Status
 * Others: None
 **********************************************/
void MapSelectDlg::OnSaveConfigParams()
{
    try
    {
        YAML::Node params = YAML::LoadFile(config_file_path.c_str());
        YAML::Node params_root = params["map_select"];

        params_root["select_map_path"]  = select_map_path_;
        params_root["vehicle_map_path"]  = vehicle_map_path_;
        qDebug() << "OnSaveConfigParams select_map_path" << QString::fromStdString(select_map_path_);

        ofstream os;
        os.open(config_file_path);
        os << params;
        os.close();
    }
    catch(...)
    {
    }
}
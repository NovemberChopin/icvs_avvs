#include "../include/qrviz.hpp"

QRviz::QRviz(QVBoxLayout *layout,QString node_name)
{
    nullmap.clear();

    this->layout=layout;
    this->nodename=node_name;


    //创建rviz容器
    render_panel_ = new rviz::RenderPanel;
    //向layout添加widget
    layout->addWidget(render_panel_);
    //初始化rviz控制对象
    manager_ = new rviz::VisualizationManager(render_panel_);
    ROS_ASSERT(manager_ != nullptr);
    
    // 获取Display组
    display_group_ = manager_->getRootDisplayGroup();
    
    //获取当前rviz控制对象的 tool控制对象
    tool_manager_=manager_->getToolManager();
    ROS_ASSERT(tool_manager_ != nullptr);

    view_manager_ = manager_->getViewManager();
    view_manager_->setRenderPanel(render_panel_);

   //初始化camera 这行代码实现放大 缩小 平移等操作
    render_panel_->initialize(manager_->getSceneManager(),manager_);
    manager_->initialize();
    tool_manager_->initialize();
    manager_->removeAllDisplays();

}

// FPS,
// ORBIT,
// THIRD_PERSON_FOLLOWER,
// TOP_DOWN_OR_THO,
// XY_OR_BIT
void QRviz::SetRvizView(RvizViewType view_type)//2-2D, 3-3D
{
    switch(view_type)
    {
        case FPS:
        {
            view_manager_->setCurrentViewControllerType("rviz/FPS");
            current_view_ = FPS;
            break;
        }
        case ORBIT:
        {
            view_manager_->setCurrentViewControllerType("rviz/Orbit");
            current_view_ = ORBIT;
            break;
        }
        case THIRD_PERSON_FOLLOWER:
        {
            view_manager_->setCurrentViewControllerType("rviz/ThirdPersonFollower");
            // view_manager_->getCurrent()->subProp("Target Frame")->setValue("/base_link");
            current_view_ = THIRD_PERSON_FOLLOWER;
            break;
        }
        case TOP_DOWN_OR_THO:
        {
            view_manager_->setCurrentViewControllerType("rviz/TopDownOrtho");
            view_manager_->getCurrent()->subProp("Angle")->setValue("0");
            view_manager_->getCurrent()->subProp("X")->setValue("0");
            view_manager_->getCurrent()->subProp("Y")->setValue("0");
            // view_manager_->getCurrent()->subProp("Target Frame")->setValue("/base_link");
            current_view_ = TOP_DOWN_OR_THO;
            break;
        }
        case XY_OR_BIT:
        {
            view_manager_->setCurrentViewControllerType("rviz/XYOrbit");
            current_view_ = XY_OR_BIT;
            break;
        }
        default:
        break;
    }
}

void QRviz::ReturnToZero()
{
    switch(current_view_)
    {
        case FPS:
        {
            break;
        }
        case ORBIT:
        {
            break;
        }
        case XY_OR_BIT:
        {
            break;
        }
        case TOP_DOWN_OR_THO:
        {
            view_manager_->getCurrent()->subProp("Near Clip Distance")->setValue("0.01");
            view_manager_->getCurrent()->subProp("Scale")->setValue("50");
            view_manager_->getCurrent()->subProp("Angle")->setValue("0");
            view_manager_->getCurrent()->subProp("X")->setValue("0");
            view_manager_->getCurrent()->subProp("Y")->setValue("0");
            break;
        }
        case THIRD_PERSON_FOLLOWER:
        {
            view_manager_->getCurrent()->subProp("Near Clip Distance")->setValue("0.01");
            view_manager_->getCurrent()->subProp("Focal Point")->setValue("0;0;0");
            view_manager_->getCurrent()->subProp("Focal Shape Size")->setValue("0.05");
            view_manager_->getCurrent()->subProp("Yaw")->setValue("0.7");
            view_manager_->getCurrent()->subProp("Pitch")->setValue("0.3");
            view_manager_->getCurrent()->subProp("Distance")->setValue("50");
            break;
        }
        default:
        break;
    }
}

QRviz::~QRviz()
{
    if (layout != nullptr && render_panel_ != nullptr)
    {
        layout->removeWidget(render_panel_);
    }
  
    if (render_panel_ != nullptr) delete render_panel_;
    if (manager_ != nullptr) delete manager_;
    
    if (current_tool != nullptr) current_tool = nullptr;
    if (tool_manager_ != nullptr) tool_manager_ = nullptr;
    
    ROS_INFO("RVIZ is shutdown");
}

///
/// \brief 获取Rviz Display树状图
///
void QRviz::GetDisplayTreeModel()
{
    rviz::PropertyTreeModel *rvizmodel = manager_->getDisplayTreeModel();
    QAbstractItemModel *model = rvizmodel;
    emit ReturnModelSignal(model);
}

///
/// \brief Rviz Display的初始化与设置
/// \param ClassID
/// \param namevalue
///
void QRviz::DisplayInit(QString ClassID, bool enabled, QMap<QString, QVariant> namevalue)
{
    int num = GetDisplayNum(ClassID);
    if (num == -1)
    {
        rviz::Display *rvizDisplay = manager_->createDisplay(ClassID, ClassID, true);
        ROS_ASSERT(rvizDisplay != nullptr);
        num = GetDisplayNum(ClassID);
    }
    if (!namevalue.empty())
    {
        QMap<QString, QVariant>::iterator it;
        for (it = namevalue.begin(); it != namevalue.end(); it++)
        {
            display_group_->getDisplayAt(num)->subProp(it.key())->setValue(it.value());
        }
    }
    display_group_->getDisplayAt(num)->setEnabled(enabled);
    manager_->startUpdate();
}
void QRviz::DisplayInit(QString ClassID, QString name, bool enabled, QMap<QString, QVariant> namevalue)
{
    int num = GetDisplayNum(ClassID, name);
    if (num == -1)
    {
        rviz::Display *rvizDisplay = manager_->createDisplay(ClassID, name, true);
        ROS_ASSERT(rvizDisplay != nullptr);
        num = GetDisplayNum(ClassID, name);
    }
    if (!namevalue.empty())
    {
        QMap<QString, QVariant>::iterator it;
        for (it = namevalue.begin(); it != namevalue.end(); it++)
        {
            display_group_->getDisplayAt(num)->subProp(it.key())->setValue(it.value());
        }
    }
    display_group_->getDisplayAt(num)->setEnabled(enabled);
    manager_->startUpdate();
}

///
/// \brief 删除Display
/// \param name
///
void QRviz::RemoveDisplay(QString name)
{
    int num = GetDisplayNumName(name);
    if (num == -1)
    {
        return ;
    }
    delete display_group_->getDisplayAt(num);
//    rvizDisplays_.removeAt(num);
}
void QRviz::RemoveDisplay(QString ClassID, QString name)
{
    int num = GetDisplayNum(ClassID, name);
    if (num == -1)
    {
        return ;
    }
    delete display_group_->getDisplayAt(num);
//    rvizDisplays_.removeAt(num);
}

///
/// \brief 重命名Display
/// \param oldname
/// \param newname
///
void QRviz::RenameDisplay(QString oldname, QString newname)
{
    int num = GetDisplayNumName(oldname);
    if (num == -1)
    {
        return ;
    }
    display_group_->getDisplayAt(num)->setName(newname);
}

///
/// \brief 导出 RVIZ Display 配置
/// \param path
///
void QRviz::OutDisplaySet(QString path)
{
    if (!path.isEmpty())
    {
        if (manager_ == nullptr)
        {
            return;
        }
        rviz::Config con;
        manager_->save(con);
        rviz::YamlConfigWriter yamlconfigwriter;
        yamlconfigwriter.writeFile(con, path);
    }
}

///
/// \brief 导入 RVIZ Display 配置
/// \param path
///
void QRviz::ReadDisplaySet(QString path)
{
    if (!path.isEmpty())
    {
        if (manager_ == nullptr)
        {
            return;
        }
        rviz::YamlConfigReader yamlconfigreader;
        rviz::Config con;
        yamlconfigreader.readFile(con, path);
        manager_->load(con);
    }
}

///
/// \brief 根据Display的ClassID(和name)获得Display的序号
/// \param ClassID
/// \return 
///
int QRviz::GetDisplayNum(QString ClassID)
{
    int num = -1;
    for (int i = 0; i < display_group_->numDisplays(); i++)
    {
        if (display_group_->getDisplayAt(i) != nullptr)
        {
            if (ClassID == display_group_->getDisplayAt(i)->getClassId())
            {
                num = i;
                break;
            }
        }
    }
    return num;
}
int QRviz::GetDisplayNum(QString ClassID, QString name)
{
    int num = -1;
    for (int i = 0; i < display_group_->numDisplays(); i++)
    {
        if (display_group_->getDisplayAt(i) != nullptr)
        {
            if (ClassID == display_group_->getDisplayAt(i)->getClassId() && name == display_group_->getDisplayAt(i)->getName())
            {
                num = i;
                break;
            }
        }
    }
    return num;
}
int QRviz::GetDisplayNumName(QString name)
{
    int num = -1;
    for (int i = 0; i < display_group_->numDisplays(); i++)
    {
        if (display_group_->getDisplayAt(i) != nullptr)
        {
            if (name == display_group_->getDisplayAt(i)->getName())
            {
                num = i;
                break;
            }
        }
    }
    return num;
}

//设置全局显示
 void QRviz::SetGlobalOptions(QString frame_name,QColor backColor,int frame_rate)
 {
     manager_->setFixedFrame(frame_name);
     manager_->setProperty("Background Color",backColor);
     manager_->setProperty("Frame Rate",frame_rate);
     manager_->startUpdate();
 }

// "rviz/MoveCamera";
// "rviz/Interact";
// "rviz/Select";
// "rviz/SetInitialPose";
// "rviz/SetGoal";
 //设置机器人导航初始位置
 void QRviz::Set_Pos()
 {
     //获取设置Pos的工具
     //添加工具

     current_tool= tool_manager_->addTool("rviz/SetInitialPose");
     //设置当前使用的工具为SetInitialPose（实现在地图上标点）
     tool_manager_->setCurrentTool( current_tool );
     manager_->startUpdate();

//     tool_manager_->setCurrentTool()

 }
 //设置机器人导航目标点
 void QRviz::Set_Goal()
 {
     //添加工具
     current_tool= tool_manager_->addTool("rviz/SetGoal");
     //设置goal的话题
     rviz::Property* pro= current_tool->getPropertyContainer();
     pro->subProp("Topic")->setValue("/move_base_simple/goal");
     //设置当前frame
     // manager_->setFixedFrame("map");
     //设置当前使用的工具为SetGoal（实现在地图上标点）
     tool_manager_->setCurrentTool( current_tool );

     manager_->startUpdate();

 }
 void QRviz::Set_MoveCamera()
 {
     //获取设置Pos的工具
     //添加工具

     current_tool= tool_manager_->addTool("rviz/MoveCamera");
     //设置当前使用的工具为SetInitialPose（实现在地图上标点）
     tool_manager_->setCurrentTool( current_tool );
     manager_->startUpdate();
 }
 void QRviz::Set_Select()
 {
     //获取设置Pos的工具
     //添加工具

     current_tool= tool_manager_->addTool("rviz/Select");
     //设置当前使用的工具为SetInitialPose（实现在地图上标点）
     tool_manager_->setCurrentTool( current_tool );
     manager_->startUpdate();
 }

 void QRviz::addTool( rviz::Tool* )
 {

 }
void QRviz::createDisplay(QString display_name,QString topic_name)
{
    
}
void QRviz::run()
{

}

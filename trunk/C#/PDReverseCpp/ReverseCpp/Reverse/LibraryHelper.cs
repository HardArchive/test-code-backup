namespace LZ.Reverse
{
    using PdCommon;
    using PdOOM;
    using PdWSP;
    using System;
    using System.Collections;

    internal class LibraryHelper
    {
        private const string MSG_INDEXING = "... indexation of the library \"{0}\"";

        private static void IndexModelClassifiers(Model aModel)
        {
            Queue queue = new Queue();
            Hashtable hashtable = new Hashtable();
            queue.Enqueue(aModel);
            while (queue.Count != 0)
            {
                PdOOM.BasePackage key = (PdOOM.BasePackage) queue.Dequeue();
                if (!hashtable.Contains(key))
                {
                    string code;
                    switch (key.ClassKind)
                    {
                        case 0x18112060:
                            code = "";
                            hashtable.Add(key, code);
                            foreach (Classifier classifier in ((Model) key).Classifiers)
                            {
                                IndexOneClassifier(classifier, code);
                            }
                            break;

                        case 0x18112061:
                        {
                            PdOOM.BasePackage parent = (PdOOM.BasePackage) key.Parent;
                            string str2 = (string) hashtable[parent];
                            if (key.UseParentNamespace)
                            {
                                code = str2;
                            }
                            else if (str2 == "")
                            {
                                code = key.Code;
                            }
                            else
                            {
                                code = str2 + "." + key.Code;
                            }
                            hashtable.Add(key, code);
                            foreach (Classifier classifier2 in ((Package) key).Classifiers)
                            {
                                IndexOneClassifier(classifier2, code);
                            }
                            break;
                        }
                    }
                    foreach (Package package3 in key.Packages)
                    {
                        if (!package3.IsShortcut())
                        {
                            queue.Enqueue(package3);
                        }
                    }
                }
            }
        }

        private static void IndexOneClassifier(Classifier aCls, string aPrefix)
        {
            if ((aCls != null) && !aCls.IsShortcut())
            {
                ArrayList list = new ArrayList();
                string code = aCls.Code;
                Classifier item = aCls;
                while (true)
                {
                    if (list.Contains(item))
                    {
                        break;
                    }
                    list.Add(item);
                    PdOOM.BaseObject containerClassifier = item.ContainerClassifier;
                    if (containerClassifier == null)
                    {
                        break;
                    }
                    item = (Classifier) containerClassifier;
                    code = item.Code + "+" + code;
                }
                if (aPrefix != "")
                {
                    code = aPrefix + "." + code;
                }
                TypeMapping.DefineExtraType(code, aCls);
            }
        }

        public static void OpenLibrary(Application anApp, string aName, string aLang)
        {
            PdOOM.BaseObject modelObject;
            WorkspaceModel model = RetrieveModelByFile(anApp, aName);
            if (model == null)
            {
                Workspace activeWorkspace = (Workspace) anApp.ActiveWorkspace;
                if (activeWorkspace == null)
                {
                    throw new Exception("Unable to reach the Workspace");
                }
                modelObject = (PdOOM.BaseObject) activeWorkspace.AddDocument(aName, -1);
                if (modelObject == null)
                {
                    return;
                }
                model = (WorkspaceModel) modelObject;
            }
            Model aModel = null;
            if (model.ModelKind == 0x18112060)
            {
                modelObject = (PdOOM.BaseObject) model.ModelObject;
                if (modelObject == null)
                {
                    long interactiveMode = (long) anApp.InteractiveMode;
                    anApp.InteractiveMode = InteractiveModeValue.im_Batch;
                    try
                    {
                        modelObject = (PdOOM.BaseObject) model.Open();
                    }
                    catch (Exception exception)
                    {
                        if (LZ.Reverse.Info._bDebug)
                        {
                            LZ.Reverse.Info.Write(new string[] { exception.StackTrace });
                        }
                    }
                    finally
                    {
                        anApp.InteractiveMode = (InteractiveModeValue) ((int) interactiveMode);
                    }
                }
                if (modelObject != null)
                {
                    aModel = (Model) modelObject;
                    ObjectLanguage objectLanguage = (ObjectLanguage) aModel.ObjectLanguage;
                    if (objectLanguage.Name == aLang)
                    {
                        string name;
                        if (aModel.Name != aModel.Code)
                        {
                            name = aModel.Name + " (" + aModel.Code + ")";
                        }
                        else
                        {
                            name = aModel.Name;
                        }
                        LZ.Reverse.Info.Write(new string[] { "... indexation of the library \"{0}\"", name });
                        IndexModelClassifiers(aModel);
                    }
                }
            }
        }

        private static WorkspaceModel RetrieveModelByFile(Application anApp, string aFileName)
        {
            Workspace activeWorkspace = (Workspace) anApp.ActiveWorkspace;
            if (activeWorkspace == null)
            {
                throw new Exception("Unable to reach the Workspace");
            }
            Stack stack = new Stack();
            stack.Push(activeWorkspace);
            ArrayList list = new ArrayList();
            while (stack.Count != 0)
            {
                WorkspaceFolder item = (WorkspaceFolder) stack.Pop();
                if (!list.Contains(item))
                {
                    list.Add(item);
                    foreach (WorkspaceElement element in item.Children)
                    {
                        switch (element.ClassKind)
                        {
                            case -1262200149:
                            {
                                WorkspaceDocument document = (WorkspaceDocument) element;
                                if (document.Filename.ToLower() == aFileName.ToLower())
                                {
                                    return (WorkspaceModel) document;
                                }
                                break;
                            }
                            case 0x42b53a08:
                                stack.Push(element);
                                break;
                        }
                    }
                }
            }
            return null;
        }
    }
}


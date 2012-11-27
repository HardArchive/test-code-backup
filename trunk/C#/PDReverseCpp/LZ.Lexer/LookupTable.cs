namespace LZ.Lexer
{
    using System;
    using System.Globalization;
    using System.Reflection;

    public class LookupTable
    {
        private bool casesensitive;
        private int length;
        private Node root = new Node(-1, null);

        public LookupTable(bool casesensitive)
        {
            this.casesensitive = casesensitive;
        }

        public int Count
        {
            get
            {
                return this.length;
            }
        }

        public int this[string keyword]
        {
            get
            {
                Node root = this.root;
                if (!this.casesensitive)
                {
                    keyword = keyword.ToUpper(CultureInfo.InvariantCulture);
                }
                for (int i = 0; i < keyword.Length; i++)
                {
                    int index = keyword[i] % 'Ā';
                    root = root.leaf[index];
                    if (root == null)
                    {
                        return -1;
                    }
                    if (keyword == root.word)
                    {
                        return root.val;
                    }
                }
                return -1;
            }
            set
            {
                Node root = this.root;
                Node node2 = this.root;
                if (!this.casesensitive)
                {
                    keyword = keyword.ToUpper(CultureInfo.InvariantCulture);
                }
                this.length++;
                for (int i = 0; i < keyword.Length; i++)
                {
                    int index = keyword[i] % 'Ā';
                    char ch1 = keyword[i];
                    node2 = node2.leaf[index];
                    if (node2 == null)
                    {
                        root.leaf[index] = new Node(value, keyword);
                        return;
                    }
                    if ((node2.word != null) && (node2.word.Length != i))
                    {
                        string word = node2.word;
                        int val = node2.val;
                        node2.val = -1;
                        node2.word = null;
                        this[word] = val;
                    }
                    if (i == (keyword.Length - 1))
                    {
                        node2.word = keyword;
                        node2.val = value;
                        return;
                    }
                    root = node2;
                }
            }
        }

        private class Node
        {
            public LookupTable.Node[] leaf = new LookupTable.Node[0x100];
            public int val;
            public string word;

            public Node(int val, string word)
            {
                this.word = word;
                this.val = val;
            }
        }
    }
}

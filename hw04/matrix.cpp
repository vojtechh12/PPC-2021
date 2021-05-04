/*** PPC - HW03 ***/
/* Tento soubor se odevzdava. Zde implementujte vsechny potrebne funkce */

#include "matrix.hpp"

using namespace std;

/*** CONSTRUCTORS & DESTRUCTOR ***/
Matrix::Matrix(int nrow, int ncol) : nrows(nrow), ncols(ncol)
{
    if ((nrows<0) || (ncols<0))
        throw matrix_negative_size();
    data = vector<value>(nrows*ncols, 0);
}

Matrix::Matrix(int rowscols) : Matrix::Matrix(rowscols, rowscols)
{
    // tady uz nic neni potreba
}

Matrix::Matrix(const Matrix &m) : nrows(m.nrows), ncols(m.ncols)
{
    this->data = std::vector<value>(m.data);
}

Matrix::Matrix(const std::vector<value> &v)
{
    int n = v.size();
    nrows = n;
    ncols = n;
    data = vector<value>(nrows*ncols, 0);

    for (int i = 0; i < n; i++) {
        this->data[i*n + i] = v[i];
    }
}

Matrix::~Matrix()
{
    //nepouzivame dynamicky alok pamet, destruktor nedela nic
}

/*** L1 Matrix:: FUNCTIONS ***/
void Matrix::zeros()
{
    Matrix::uniform(0);
}

void Matrix::uniform(value element)
{
    this->data = vector<value>(nrows*ncols, element);
}

void Matrix::eye()
{
    Matrix::zeros();
    int maxdim = min(this->ncols, this->nrows);
    for (int i = 0; i < maxdim; i++) 
    {
        this->data.at(i*ncols + i) = 1;
    }
}

void Matrix::bias(value b)
{
    for(int row = 0; row < this->nrows; row++)
    {
        for (int col = 0; col < this->ncols; col++)
        {
            this->data.at(row*ncols + col) += b;
        }
    }

}

/*** L2 Matrix:: FUNCTIONS ***/
void Matrix::scale(value s)
{
    for (size_t i = 0; i < this->data.size(); i++)
    {
        this->data.at(i) *= s;
    }
}

void Matrix::transpose()
{
    Matrix transp = Matrix(this->ncols, this->nrows);
    for(int row = 0; row < this->nrows; row++)
    {
        for (int col = 0; col < this->ncols; col++)
        {
            transp(col, row) = this->data.at(row*ncols + col);
        }
    }
    *this = transp;
}

/*** L3 Matrix:: FUNCTIONS ***/
void Matrix::changedim(int rows, int cols)
{
    if ((rows < 0) || (cols < 0)) 
        throw matrix_negative_size();

    Matrix nova = Matrix(rows, cols);
    int rowstop = 0;
    int colstop = 0;

    rows > (int)this->nrows ? rowstop = this->nrows : rowstop = rows;
    cols > (int)this->ncols ? colstop = this->ncols : colstop = cols;

    int i = 0;
    for (; i < rowstop; i++) {
        int  j = 0;
        for (; j < colstop; j++) {
            nova(i, j) = (*this)(i,j);
        }
        // prip doplneni nulami
        while (j < cols) {
            nova(i,j) = 0;
            j++;
        }
    }
    // prip doplneni radku
    while (i < rows) {
        for (int z = 0; z < cols; z++) {
            nova(i,z) = 0;
        }
        i++;
    }
    *this = nova;
}

/*** L1 Matrix:: Operators ***/
value Matrix::operator () (int row, int col) const
{
    if ((row >= this->nrows) || (row < 0)) // row a col indexuji od nuly
        throw std::out_of_range("Row index is out of range");
    if ((col >= this->ncols) || (col < 0))
    throw std::out_of_range("Column index is out of range");
    return this->data.at(row*ncols + col);
}

value & Matrix::operator () (int row, int col)
{
    if ((row >= this->nrows) || (row < 0)) // row a col indexuji od nuly
        throw std::out_of_range("Row index is out of range");
    if ((col >= this->ncols) || (col < 0))
        throw std::out_of_range("Column index is out of range");
    return this->data.at(row*ncols + col);
}

bool Matrix::operator==(const Matrix &m)
{
    if (this->nrows != m.nrows)
        return false;
    if (this->ncols != m.ncols)
        return false;
    for (int i  = 0; i < (this->nrows * this->ncols); i++)
    {
        if (this->data.at(i) != m.data.at(i))
        return false;
    }
    return true;
}

Matrix & Matrix::operator=(const Matrix &m)
{
    if (*this == m)
        return *this;
    this->nrows = m.nrows;
    this->ncols = m.ncols;
    this->data = vector<value>(m.data);
    return *this;
}

/*** L3 Matrix:: Operators ***/
std::vector<value> Matrix::operator () (void) const
{
    std::vector<value> dat;
    int n = 0;
    this->nrows > this->ncols ? n=this->ncols : n=this->nrows;
    dat.resize(n);

    //int nr = this->nrows;
    //int nc = this->ncols;


    for (int i = 0; i < n; i++) {
        dat[i] = this->data[(i*this->ncols) + i];
    }
    
    return dat; 
}


/*** L1 NON-MEMBER FUNCTIONS/OPERATORS ***/
std::ostream & operator<<(std::ostream &os, const Matrix &m)
{
    if ((m.nrows == 0 ) || (m.ncols == 0))
        os << std::endl;
    else
    {
        for(int row = 0; row < m.nrows; row++)
        {
            for (int col = 0; col < m.ncols; col++)
            {
                os << std::setw(5) << m(row, col) << " ";
            }
            os << std::endl;
        }
    }
    return os;
}

stringstream nactiRadek(ifstream & vstup)
{
    string radek;
    getline(vstup, radek);
    stringstream ss(radek);
    return ss;
}

void loadMatrixFile(Matrix & m, string fname)
{
    std::ifstream vstup(fname);
    if (vstup.is_open())
    {
        stringstream ss;
        ss = nactiRadek(vstup);
        string slovo;
        vector<int> headerval;
        while (ss >> slovo)
        {
            try 
            {
                headerval.push_back(stoi(slovo));
            }
            catch(std::invalid_argument &e)
            {
                throw matrix_in_bad_header();
            }
        }
        if (headerval.size() != 2)
            throw matrix_in_bad_header();
            // tady hotova hlavicka s rozmery
        Matrix x(headerval[0], headerval[1]);
        if ((x.nrows == 0) || (x.ncols == 0))
        {
            // osetruju matici s nulovym rozmerem
            m = x;
            return;
        }
        for (int row = 0; row < x.nrows; row++)
        {
            ss = nactiRadek(vstup);
            int col = 0;
            while ((ss >> slovo) && (col < x.ncols))
            {
                try 
                {
                    x(row, col) = stoi(slovo);
                }
                catch(std:: invalid_argument &e)
                {
                    throw matrix_in_bad_body();
                }
                col++;
            }
            if(col < x.ncols)
                throw matrix_in_bad_body();
        }
        m = x;
    }
    else
        throw cannot_open_file();
}

Matrix operator+(const Matrix &lhs, const Matrix &rhs)
{
    if ((lhs.nrows != rhs.nrows) || (lhs.ncols != rhs.ncols))
        throw matrix_bad_sizes();
    Matrix vysl(lhs.nrows, lhs.ncols);
        for(int row = 0; row < lhs.nrows; row++)
        {
            for (int col = 0; col < lhs.ncols; col++)
            {   
                vysl(row, col) = lhs(row, col) + rhs(row, col);
            }
        
        }
    return vysl;
}

Matrix operator+(const value & lhs, const Matrix &rhs)
{
    Matrix vysl(rhs);
    vysl.bias(lhs);
    return vysl;
}

Matrix operator+(const Matrix &lhs, const value & rhs) 
{
    return rhs + lhs; // prevedli jsme na uz hotovou ulohu value + matrix
}

Matrix operator-(const Matrix &lhs, const Matrix &rhs)
{
    if ((lhs.nrows != rhs.nrows) || (lhs.ncols != rhs.ncols))
        throw matrix_bad_sizes();
    Matrix vysl(lhs.nrows, lhs.ncols);
    for(int row = 0; row < lhs.nrows; row++)
    {
        for (int col = 0; col < lhs.ncols; col++)
        {   
            vysl(row, col) = lhs(row, col) - rhs(row, col);
        }
    
    }
    return vysl;

}

Matrix operator-(const value & lhs, const Matrix &rhs)
{
    Matrix vysl(rhs.nrows, rhs.ncols);
    for(int row = 0; row < rhs.nrows; row++)
    {
        for (int col = 0; col < rhs.ncols; col++)
        {   
            vysl(row, col) = lhs - rhs(row, col);
        }
    
    }
    return vysl;

}

Matrix operator-(const Matrix &lhs, const value & rhs) 
{
    Matrix vysl(lhs);
    int dec = -rhs;
    vysl.bias(dec);
    return vysl;
}

/*** L2 NON-MEMBER FUNCTIONS/OPERATORS ***/

Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    if (lhs.ncols != rhs.nrows)
        throw matrix_bad_sizes();
    Matrix vysl = Matrix(lhs.nrows, rhs.ncols);
    for (int r = 0; r < lhs.nrows; r ++) {
        for (int c = 0; c < rhs.ncols; c++) {
            for (int k = 0; k < lhs.ncols; k++) {
                vysl(r,c) +=  lhs(r,k) * rhs(k,c);
            }
        }
    }
    return vysl;
}

Matrix operator*(const value & lhs, const Matrix &rhs)
{
    Matrix vysl = rhs;
    vysl.scale(lhs);
    return vysl;
}

Matrix operator*(const Matrix &lhs, const value & rhs) 
{
    return rhs * lhs;
}

Matrix hadamard(const Matrix &lhs, const Matrix &rhs)
{
    if ((lhs.ncols != rhs.ncols) || (lhs.nrows != rhs.nrows) )
        throw matrix_bad_sizes();
    Matrix vysl = Matrix(lhs.nrows, lhs.ncols);
    for (size_t i = 0; i < lhs.data.size(); i++)
    {
        vysl.data.at(i) = lhs.data.at(i) * rhs.data.at(i);
    }
    return vysl;
}

Matrix power(const Matrix &m, unsigned int pow)
{
    if (m.nrows != m.ncols)
        throw matrix_bad_sizes();
    Matrix vysl = m;
    if (pow == 0) {
        vysl.eye();
        return vysl;
    }
    else {
        // cycle
        for (unsigned int i = 1; i < pow; i++) {
            vysl = vysl * m;
        }
        return vysl;
    }
}

/*** L3 NON-MEMBER FUNCTIONS/OPERATORS ***/
Matrix horzcat(const Matrix &lhs, const Matrix &rhs)
{
    if (lhs.nrows != rhs.nrows)
        throw matrix_bad_sizes();
    Matrix vysl = Matrix(lhs.nrows, (lhs.ncols + rhs.ncols));

    for (int i = 0; i < vysl.nrows; i++) {
        for (int j = 0; j < lhs.ncols; j++) {
            vysl(i,j) = lhs(i,j);
        }
        for (int k = 0; k < rhs.ncols; k++) {
            vysl(i, lhs.ncols + k) = rhs(i,k);
        }
    }
    return vysl;
}

Matrix vertcat(const Matrix &top, const Matrix &bot)
{
    if (top.ncols != bot.ncols)
        throw matrix_bad_sizes();

    Matrix vysl = Matrix((top.nrows + bot.nrows), top.ncols);
    for (int i = 0; i < top.nrows; i++) {
        for (int j = 0; j < top.ncols; j++) {
            vysl(i,j) = top(i,j);
        }
    }
    for (int i = 0; i < bot.nrows; i++) {
        for (int j = 0; j < bot.ncols; j++) {
            vysl(top.nrows + i, j) = bot(i,j);
        }
    }
    return vysl;
}

Matrix blkdiag (const Matrix & lhs, const Matrix & rhs)
{
    if ((lhs.nrows != lhs.ncols) || (rhs.nrows!= rhs.ncols))
        throw matrix_bad_sizes();

    Matrix vysl = Matrix(lhs.nrows + rhs.nrows, lhs.ncols + rhs.ncols);
    // right top block of zeros
    Matrix rtblank = Matrix(lhs.nrows, rhs.ncols);
    // merge with lefttop block alias lhs
    Matrix topmerged = horzcat(lhs, rtblank);

    //left bottom block of zeros
    Matrix lbblank = Matrix(rhs.nrows, lhs.ncols);
    // merge with rightbottom block alias rhs
    Matrix botmerged = horzcat(lbblank, rhs);

    //merge top and bottom
    vysl = vertcat(topmerged, botmerged);
    return vysl;
}

Matrix blkdiag (const value & lhs, const Matrix & rhs) 
{
    if (rhs.nrows != rhs.ncols)
        throw matrix_bad_sizes();
    Matrix left = Matrix(1,1);
    left.bias(lhs);

    // prevedu na jiz hotovou ulohu blkdiag(matrix, matrix)
    Matrix vysl = blkdiag(left, rhs);
    return vysl;
}

Matrix blkdiag (const Matrix & lhs, const value & rhs) 
{
    if (lhs.nrows != lhs.ncols)
        throw matrix_bad_sizes();
    Matrix right = Matrix(1,1);
    right.bias(rhs);

    //prevedu na jiz znamou ulohu
    Matrix vysl = blkdiag(lhs, right);
    return vysl;
}

Matrix blkdiag (const value & lhs, const value & rhs) 
{
    Matrix left = Matrix(1,1);
    Matrix right = Matrix(1,1);
    left.bias(lhs);
    right.bias(rhs);

    // prevedu na jiz znamou ulohu
    Matrix vysl = blkdiag(left, right);
    return vysl;
}

Matrix kronecker(const Matrix &lhs, const Matrix &rhs)
{
    Matrix vysl;
    Matrix rowOfMat;
    Matrix tmpM;
    for (int i = 0; i < lhs.nrows; i++) {
        
        for (int j = 0; j < lhs.ncols; j++) {
            tmpM = lhs(i,j) * rhs;
            if (j == 0) rowOfMat = tmpM;
            else rowOfMat = horzcat(rowOfMat, tmpM);
        }
        if (i == 0) vysl = rowOfMat;
        else vysl = vertcat(vysl, rowOfMat);
    }
    return vysl;
}




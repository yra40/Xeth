#pragma once

#include <map>
#include <list>

#include <boost/thread.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include "ethrpc/BlockChain.hpp"
#include "ethrpc/Block.hpp"
#include "ethrpc/Transaction.hpp"
#include "ethrpc/Collection.hpp"

#include "ScanCriterion.hpp"
#include "ScanProgress.hpp"
#include "ScanResult.hpp"

#include "detail/InterruptionPoint.hpp"

namespace Xeth{

typedef Ethereum::Connector::Collection<Ethereum::Connector::Transaction> TransactionCollection;

using Ethereum::Connector::BlockChain;
using Ethereum::Connector::Block;
using Ethereum::Connector::Transaction;
using Ethereum::Connector::Collection;


class ScanCriteria : public QObject
{
    Q_OBJECT

    public:
        class Iterator;

    public:

        ScanCriteria(size_t limit);
        ScanCriteria();
        ~ScanCriteria();

        void setLimit(size_t);

        template<class Criterion, class Arg>
        void addCriterion(size_t minBlock, const Arg &);

        template<class Criterion, class Arg1, class Arg2>
        void addCriterion(size_t minBlock, const Arg1&, const Arg2&);

        template<class Criterion, class Arg1, class Arg2, class Arg3>
        void addCriterion(size_t minBlock, const Arg1 &, const Arg2 &, const Arg3&);

        size_t parse(BlockChain &, ScanResult &);

        template<class BlockChain, class Progress>
        size_t parse(BlockChain &, ScanResult &, Progress &);

        void clear();

        Iterator begin() const;
        Iterator end() const;

        size_t size() const;

    signals:
        void Data(const Xeth::PartialScanResult &);
        void Done(const Xeth::ScanResult &);


    private:
        typedef std::list<std::pair<size_t, ScanCriterion *> > Container;
        class CriterionCompare;

    private:
        ScanCriteria(const ScanCriteria &);
        void processBlock(BlockChain &, size_t index, Ethereum::Connector::Block &, Container::iterator, ScanResult &);
        void addCriterion(size_t minBlock, ScanCriterion *);
        void registerMetaType();

    private:
        Container _criteria;
        bool _interrupted;
        size_t _limit;

};


class ScanCriteria::CriterionCompare
{
    public:
        bool operator()(const std::pair<size_t, ScanCriterion *> &, size_t);
};


class ScanCriteria::Iterator : public boost::iterator_adaptor<
      ScanCriteria::Iterator,
      ScanCriteria::Container::const_iterator,
      const ScanCriterion &,
      boost::forward_traversal_tag,
      const ScanCriterion & >
{
    public:
        typedef boost::iterator_adaptor<
          ScanCriteria::Iterator,
          ScanCriteria::Container::const_iterator,
          const ScanCriterion &,
          boost::forward_traversal_tag,
          const ScanCriterion & > Base;

    public:
        Iterator();
        explicit Iterator(const ScanCriteria::Container::const_iterator &);
        size_t getBlockIndex() const;

    private:
        friend class boost::iterator_core_access;
        const ScanCriterion & dereference() const;
};



}

#include "ScanCriteria.ipp"

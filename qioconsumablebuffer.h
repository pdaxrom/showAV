#ifndef QIOCONSUMABLEBUFFER_H
#define QIOCONSUMABLEBUFFER_H

#include <QIODevice>
#include <QByteArray>

//#define QIO_CONSUMABLE_DEBUG

#ifdef QIO_CONSUMABLE_DEBUG
#include <QDebug>
#include <QDateTime>

#define QIO_LOG_X( STREAM ) { int _now=QDateTime::currentMSecsSinceEpoch();qDebug() << "(" << (_now-QIOConsumableBuffer::_last_millis()) << "ms) " << __PRETTY_FUNCTION__ << ": " << STREAM; QIOConsumableBuffer::_last_millis()=QDateTime::currentMSecsSinceEpoch(); }
#else
#define QIO_LOG_X( STREAM ) {}
#endif

/**
     * Interface for QIO buffers
     * @date 5/22/2018
     * @author Michael A. Leonetti
     * @copyright Sonarcloud, 2018
     */
class QIOConsumableBuffer : public QIODevice
{
public:

#ifdef QIO_CONSUMABLE_DEBUG
    static int &_last_millis()
    {
        static int lm = QDateTime::currentMSecsSinceEpoch();

        return( lm );
    }
#endif

    /**
             * C-tor
             */
    QIOConsumableBuffer( QObject *parent=nullptr ) :
        QIODevice( parent )
    {
    }

    /**
             * We are considered sequential
             */
    bool isSequential() const override
    {
        QIO_LOG_X( "Check" );
        return( true );
    }

    /**
             * Whaa
             */
    qint64 bytesAvailable() const override
    {
        qint64 available = byte_array_.size() + QIODevice::bytesAvailable();
        QIO_LOG_X( "Enter function#1=" << available <<  ", errorString=" << errorString() );
        QIO_LOG_X( "Enter function#2=" << byte_array_.size() <<  ", errorString=" << errorString() );

        return( available );
    }

    /**
             * Our size
             */
    qint64 size() const override
    {
        QIO_LOG_X( byte_array_.size() );

        return( byte_array_.size() );
    }

    /**
             * When we are reading from the buffer
             */
    qint64 readData( char *data, qint64 maxlen ) override
    {
        QIO_LOG_X( "Requested: " << maxlen );

        // Correct size
        maxlen = std::min( maxlen, qint64( byte_array_.size() ) );

        QIO_LOG_X( "Have: " << maxlen );

        if( maxlen>0 )
        {
            // Copy
            memcpy( data, byte_array_.constData(), maxlen );

            // Now remove
            byte_array_.remove( 0, maxlen );
        }

        QIO_LOG_X( "Remaining: " << byte_array_.size() << ", available: " << bytesAvailable() << ", error=" << errorString() );

        // Return how much we read
        return( maxlen );
    }

    /**
             * When we are writing to the buffer
             */
    qint64 writeData( const char *data, qint64 maxlen ) override
    {
        QIO_LOG_X( "Enter function" );

        // Add it all to the end
        byte_array_.append( data, maxlen );

        QIO_LOG_X( "Byte array is now " << byte_array_.size() );

        // Max len that
        return( maxlen );
    }
private:
    QByteArray byte_array_;
};

#endif // QIOCONSUMABLEBUFFER_H
